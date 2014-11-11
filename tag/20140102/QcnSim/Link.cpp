/**
 * @author Marcos Portnoi
 * @date October 2013
 * 
 * @copyright Copyright (C) 2013 University of Delaware.
 * @copyright QCNSim uses elements of TARVOS simulator, Copyright (C) 2005, 2006, 2007 Marcos Portnoi.
 * @par
 * This file is part of QCNSim.  QCNSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.<br>
 * QCNSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.<br>
 * You should have received a copy of the GNU Lesser General Public License
 * along with QCNSim.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Link.h"

/**
 * @brief Default constructor.
 *
 * @param nodeA Pointer to Node at one side of the link.
 * @param nodeB Pointer to Node at the other side of the link.
 * @param bandwidth Bandwidth of this link, typically in bits per second.
 * @param propagationDelay Propagation delay of this link, which is medium-dependent, typically in seconds.
 * @param simulatorGlobals Reference to SimulatorGlobals object.
 * @param scheduler Reference to Scheduler object.
 * @param name Name of this link. Default is "".
 * @param linkType Type of link.
 */
Link::Link(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB, double bandwidth, double propagationDelay, SimulatorGlobals &simulatorGlobals,
		Scheduler &scheduler, const std::string &name, LinkType linkType) : nodeA(nodeA), nodeB(nodeB), bandwidth(bandwidth),
		propagationDelay(propagationDelay), simulatorGlobals(simulatorGlobals), scheduler(scheduler), name(name), linkType(linkType),
		transmissionServer(Facility("Transmission Server Original Direction", simulatorGlobals, scheduler)), droppedPdusCountMedium(0) { // Creates one transmission server (Facility with one server).
	// If this is a duplex link, create another link object in the reverse direction, put its reference into the member variable.
	if (linkType == LinkType::DUPLEX_LINK) {
		reverseLink = std::make_shared<Link>(Link(nodeB, nodeA, bandwidth, propagationDelay, simulatorGlobals, scheduler, name, LinkType::SIMPLEX_LINK));
		reverseLink->transmissionServer.setName("Transmission Server Reverse Direction");
	}
}

/**
 * @brief Destructor.
 */
//Link::~Link() {
//}

///**
// * @brief Return reference to nodeA.
// *
// * @return Reference to Node at nodeA.
// */
//Node &Link::getnodeA() const {
//	return nodeA;
//}
//
///**
// * @brief Return reference to nodeA.
// *
// * @return Reference to Node at nodeA.
// */
//Node &Link::getnodeB() const {
//	return nodeB;
//}

/**
 * @brief Return bandwidth of link.
 *
 * @return Bandwidth.
 */
double Link::getBandwidth() const {
	return bandwidth;
}

/**
 * @brief Return propagation delay or latency of link.
 *
 * @return Propagation delay.
 */
double Link::getPropagationDelay() const {
	return propagationDelay;
}

/**
 * @brief Return name of link.
 *
 * @return Name as string.
 */
std::string Link::getName() const {
	return name;
}

/**
 * @brief Return true if link is up; false otherwise. This status is the direct status of underlying Facility.
 *
 * @return Status of link. True if up; false otherwise.
 */
bool Link::isUp() const {
	return transmissionServer.isUp();
}

/**
 * @brief Set Link up (in both directions in case of duplex links).
 *
 * @details  
 * Sets the underlying Facility up. Link's transmission server statistics and status are controlled by this facility.
 */
void Link::setUp() {
	transmissionServer.setUp();
	if (linkType == LinkType::DUPLEX_LINK) {
		getReverseLink()->setUp();
	}
}

/**
 * @brief Set Link down (in both directions in case of duplex links).
 *
 * @details  
 * Sets the underlying Facility down. Link's transmission server statistics and status are controlled by this facility.
 * Drops both PDUs in transmission and PDUs in transit through link medium.
 */
unsigned int Link::setDown() {
	return transmissionServer.setDown() + purgeInTransitQueue() + ((linkType == LinkType::DUPLEX_LINK) ? getReverseLink()->setDown() : 0);
}

/**
 * @brief Get current PDU in-transit queue size.
 *
 * @return inTransitQueue size.
 */
unsigned int Link::getInTransitQueueSize() const {
	return inTransitQueue.size();
}

/**
 * @brief Requests transmission a PDU to (or through) the link.
 *
 * @details   
 * The transmission is the first step in fully moving one PDU from the source node to a destination node connected by a link.
 * The transmission puts the PDU into the link medium. After transmission, the PDU must then propagate through the link medium until reaching the
 * destination node.
 *
 * This function checks whether the "previous" and "next" nodes within the PDU fields are in fact connected through this link, or through the reverse link. It will
 * activate the appropriate forward or reverse link, or return an ERROR message otherwise.
 *
 * The link is modelled with a Facility with one server, acting as the transmission server. The service time is a function of the link bandwidth and PDU size, such that:
 *
 * service_time = PDU_size / bandwidth
 *
 * Connected to the output of the transmission server, there is a delay center or infinite capacity server. This delay center models the propagation delay of the link.
 * The complete handling of a PDU through a link will be composed of three events:
 *
 * 1. REQUEST_PDU_TRANSMISSION_AT_LINK: if successful, the next END_TRANSMISSION_PROPAGATE_PDU_AT_LINK event must be scheduled for the link. Otherwise, the PDU was probably
 *	 enqueued or dropped.
 * 2. END_TRANSMISSION_PROPAGATE_PDU_AT_LINK: if the previous event was successful, this event is scheduled. This event will cause the Link transmission server to release
 *    the PDU and then the PDU will commence propagation in the link. This is done by scheduling a PDUTOKEN_ARRIVAL_AT_NODE after the fixed-time propagation delay.
 * 3. PDUTOKEN_ARRIVAL_AT_NODE: scheduled after transmission and release of PDU from Link's transmission server. At this event, the PDU has arrived at the destination node.
 * 4. ERROR_LINK_DOES_NOT_CONNECT_NODES: PDU's fields "previous" and "next" nodes are not connected through this link or through the reverse link. The PDU will not be transmitted,
 *   unless verifyNodeConsistency is false, in which case the PDU will be transmitted by the forward link without consistency check.
 *
 * @param transmitEventType Type of event to schedule for transmission. If PDU is enqueued, this event will be used by Facility class.
 * @param endTransmitEventType Type of event to schedule for after transmission, typically a release/propagation event.
 * @param pdu Protocol Data Unit to transmit.
 *
 * @return Return type or result of transmission request.
 */
LinkReturnType Link::transmitPdu(EventType transmitEventType, EventType endTransmitEventType, std::shared_ptr<const ProtocolDataUnit> pdu) {
	// Consistency check. If nodes in PDU's previous and next fields are not connected by this node, then refuse transmission and return error state.
	if (pdu->previous == nodeA && pdu->next == nodeB) { // Forward link (this link) connects the nodes. Use it.
		double transmissionTime = pdu->getPduSize() * 8.0 / bandwidth; // Converts PDU size to bits and calculates transmission time based on bandwidth.
		// Request service for PDU at Facility (transmission server).
		FacilityReturnType serviceRequestResult = transmissionServer.request(pdu, transmitEventType);
		// Test outcomes.
		if (serviceRequestResult == FacilityReturnType::TOKEN_PUT_IN_SERVICE) {
			scheduler.schedule(Event(transmissionTime, endTransmitEventType, pdu)); // Successful; schedule next event to release/propagate.
			return LinkReturnType::PDU_IN_TRANSMISSION_NEXT_EVENT_SCHEDULED;
		} else if (serviceRequestResult == FacilityReturnType::FACILITY_BUSY_QUEUE_FULL_TOKEN_DROPPED) { // Facility queue full, PDU was dropped; update statistics.
			// The statistics here are controlled by the Facility class.
			return LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED;
		} else if (serviceRequestResult == FacilityReturnType::FACILITY_DOWN_TOKEN_DROPPED) { // Facility down (thus link is also down); PDU dropped, update stats of Node?
			// Statistics here are controlled by the Facility class.
			return LinkReturnType::LINK_DOWN_PDU_DROPPED;
		} else if (serviceRequestResult == FacilityReturnType::FACILITY_BUSY_TOKEN_ENQUEUED) { // Facility busy, token enqueued. Nothing else to do.
			return LinkReturnType::LINK_BUSY_PDU_ENQUEUED;
		} else {
			return LinkReturnType::NOT_IMPLEMENTED; // Something weird happened...
		}
	} else if (linkType == LinkType::DUPLEX_LINK && pdu->next == nodeA && pdu->previous == nodeB) { // The reverse link must be used for this PDU, in case of duplex links.
		// Call this function here in recursion in case of the reverse link in a duplex link.
		return getReverseLink()->transmitPdu(transmitEventType, endTransmitEventType, pdu);
	} else {
		// Neither the forward, nor the reverse link connect the nodes within the PDU. Return error.
		return LinkReturnType::ERROR_LINK_DOES_NOT_CONNECT_NODES;
	}	
}

/**
 * @brief Ends transmission (release PDU from Facility) and propagate PDU through this link.
 *
 * @details  
 * First, PDU is released from transmission server.
 * Then, PDU is put into inTransitQueue and an arrival event (typically PDUTOKEN_ARRIVAL_AT_NODE) is scheduled such that the PDU arrives at the destinaton
 * after the propagation delay.
 * If the link is down, the PDU is discarded and statistics, updated. This incident might never happen, since it is likely that no PDU
 * finishes transmission if or after a link is set down.
 * This function checks whether the "previous" and "next" nodes within the PDU fields are in fact connected through this link, or through the reverse link. It will
 * activate the appropriate forward or reverse link, or return an ERROR message otherwise.
 *
 * @param nextEvent Next event to schedule after propagation, typically an arrival event.
 * @param pdu PDU to propagate.
 *
 * @return Return type or result of propagation attempt.
 */
LinkReturnType Link::propagatePdu(EventType nextEvent, std::shared_ptr<const ProtocolDataUnit> pdu) {
	// Consistency check. If nodes in PDU's previous and next fields are not connected by this node, then refuse propagation and return error state.
	if (pdu->previous == nodeA && pdu->next == nodeB) { // Forward link (this link) connects the nodes. Use it.
		// Release PDU from transmission Facility.
		transmissionServer.release(pdu);
		// If link is down, discard PDU, update statistics.
		if (!isUp()) {
			++droppedPdusCountMedium;
			return LinkReturnType::LINK_DOWN_PDU_DROPPED;
		}
		// Otherwise, insert PDU into inTransitQueue and schedule next arrival event.
		inTransitQueue.push_back(pdu); // Insert into queue.
		scheduler.schedule(Event(propagationDelay, nextEvent, pdu));
		return LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED;
	} else if (linkType == LinkType::DUPLEX_LINK && pdu->next == nodeA && pdu->previous == nodeB) { // The reverse link must be used for this PDU, in case of duplex links.
		// Call this function here in recursion for the reverse link.
		return getReverseLink()->propagatePdu(nextEvent, pdu);
	} else {
		// Neither the forward, nor the reverse link connect the nodes within the PDU. Return error.
		return LinkReturnType::ERROR_LINK_DOES_NOT_CONNECT_NODES;
	}
}

/**
* Returns the busy status of this Link's transmission server (Facility).
*
* @details 
* The busy status is obtained directly from the underlying Facility (transmission server).
*
* @return True if busy, false if non-busy (not necessarily available, however; link might be down).
*/
bool Link::isTransmissionServerBusy() const {
	return transmissionServer.isBusy();
}

/**
 * @brief PDU has successfully propagated through the link; end propagation.
 *
 * @details 
 * This function must be called upon handling the event ARRIVAL of PDU at node coming from a link.
 * This is necessary such that the PDU is removed from the inTransitQueue.
 * Note that this function must be called *before* the PDU is delivered to the node, because this function identifies the proper link direction by
 * using the previous and next fields from PDU (the nodeA and nodeB from the current link that just propagated the PDU). If the PDU is delivered to
 * the node before this function is called (by calling the Node's processAndForward function), 
 * then these previous and next fields will be updated for the next link and this function will not work correctly.
 * This function checks whether the "previous" and "next" nodes within the PDU fields are in fact connected through this link, or through the reverse link. It will
 * activate the appropriate forward or reverse link, or return an ERROR message otherwise.
 *
 * @param pdu PDU that is arriving at node and thus has been successfully propagated.
 *                              the pdu without any node checking.
 *
 * @return Return type or result of propagation attempt.
 */
LinkReturnType Link::endPropagation(std::shared_ptr<ProtocolDataUnit> pdu) {
	// Consistency check. If nodes in PDU's previous and next fields are not connected by this node, then refuse end propagation and return error state.
	if (pdu->previous == nodeA && pdu->next == nodeB) { // Forward link (this link) connects the nodes. Use it.
		inTransitQueue.remove(pdu); // Removes the PDU (it should be at head of queue). If not found, does nothing. I.e., if the PDU is not really in transit here, this function will not verify it.
		return LinkReturnType::PDU_PROPAGATED;
	} else if (linkType == LinkType::DUPLEX_LINK && pdu->next == nodeA && pdu->previous == nodeB) { // The reverse link must be used for this PDU, in case of duplex links.
		// Call this function here in recursion for the reverse link.
		return getReverseLink()->endPropagation(pdu);
	} else {
		// Neither the forward, nor the reverse link connect the nodes within the PDU. Return error.
		return LinkReturnType::ERROR_LINK_DOES_NOT_CONNECT_NODES;
	}
}

/**
 * @brief Get transmission (Facility) queue size.
 *
 * @return Transmission or underlying Facility queue size.
 */
std::list<FacilityQueueElement>::size_type Link::getTransmissionQueueSize() const {
	return transmissionServer.getQueueSize();
}

/**
* @brief Get transmission (Facility) max recorded queue size.
*
* @return Transmission or underlying Facility max recorded queue size.
*/
unsigned int Link::getMaxRecordedTransmissionQueueSize() const {
	return transmissionServer.getMaxRecordedQueueSize();
}

/**
 * @brief Get transmission (Facility) queue size limit.
 *
 * @return Transmission or underlying Facility queue size limit.
 */
unsigned int Link::getTransmissionQueueSizeLimit() const {
	return transmissionServer.getQueueSizeLimit();
}

/**
 * @brief Set transmission (Facility) queue size limit (for both forward and reverse links in case of duplex links).
 *
 * @details 
 * If not set, the default here is the default inherited from Facility class (<double>max()).
 *
 * @param limit Transmission (Facility) queue size limit.
 */
void Link::setTransmissionQueueSizeLimit(unsigned int limit) {
	transmissionServer.setQueueSizeLimit(limit);
	if (linkType == LinkType::DUPLEX_LINK) {
		getReverseLink()->setTransmissionQueueSizeLimit(limit);
	}
}

/**
* @brief Return pointer to nodeA (one side of link).
*
* @return Pointer to nodeA.
*/
std::shared_ptr<Node> Link::getNodeA() const {
	return nodeA;
}

/**
* @brief Return pointer to nodeB (another side of link).
*
* @return Pointer to nodeB. 
*/
std::shared_ptr<Node> Link::getNodeB() const {
	return nodeB;
}

/**
* @brief Return pointer to nodeA, the Source node of a Simplex link.
*
* @return Pointer to nodeA.
*/
std::shared_ptr<Node> Link::getSourceNode() const {
	return getNodeA();
}

/**
* @brief Return pointer to nodeB, the Destination node of a Simplex link.
*
* @return Pointer to nodeB. 
*/
std::shared_ptr<Node> Link::getDestinationNode() const {
	return getNodeB();
}

/**
* Get count of dropped or discarded PDUs from transmission server.
*
* @details 
* A PDU will typically be discarded when the PDU arrives for service and the facility is currently down.
*
* @return Count of dropped or discarded PDUs from transmission server.
*/
unsigned int Link::getDroppedPdusCountTransmissionServer() const {
	return transmissionServer.getDroppedTokensCount();
}

/**
* Get count of dropped or discarded PDUs from medium.
*
* @details 
* A PDU will typically be discarded if the link goes down and there are PDUs in inTransitQueue.
*
* @return Count of dropped or discarded PDUs from medium or inTransitQueue.
*/
unsigned int Link::getDroppedPdusCountMedium() const {
	return droppedPdusCountMedium;
}

/**
* Get count of dropped or discarded PDUs from transmission server and medium.
*
* @details 
* The sum of dropped PDUs from transmission server and medium.
*
* @return Count of dropped or discarded PDUs from transmission server and medium.
*/
unsigned int Link::getDroppedPdusCountWholeLink() const {
	return transmissionServer.getDroppedTokensCount() + droppedPdusCountMedium;
}

/**
* @brief Purge inTransitQueue, update statistics, return number of dropped PDUs from link medium.
*
* @return Number of PDUs dropped from inTransitQueue.
*/
unsigned int Link::purgeInTransitQueue() {
	// Drop each PDU in inTransitQueue and update statistics.
	unsigned int droppedPdusHere = inTransitQueue.size(); // PDUs dropped from link medium, in transit queue.
	while (inTransitQueue.size() > 0) {
		scheduler.removeEvents(inTransitQueue.front()); // Delete events related to first PDU.
		inTransitQueue.pop_front(); // Remove first PDU.
	}
	droppedPdusCountMedium += droppedPdusHere; // Update member counter for dropped PDUs in link medium.
	return droppedPdusHere;
}

/**
 * @brief Get LinkType (type of link) of this link.
 *
 * @return Type of Link (typically simplex or duplex).
 */
LinkType Link::getLinkType() const {
	return linkType;
}

/**
 * @brief Get ReverseLink reference.
 *
 * @details 
 * Get the reference or pointer to the reverse link in case of duplex links.
 *
 * @return Reverse link smart pointer or nullptr if it is a simplex link.
 */
std::shared_ptr<Link> Link::getReverseLink() const {
	if (linkType == LinkType::SIMPLEX_LINK) {
		return nullptr; // Make sure a nullptr is returned in case of Simplex links, regardless of the contents of member reverseLink.
	} else {
		return reverseLink;
	}
}

/**
 * @brief Verifies whether this link connects parameter nodeA and nodeB.
 *
 * @details 
 * Returns true if nodeA is connected to nodeB through this link. The nodes are parameters. Returns false otherwise.
 * Note that direction matters here. The direction is from nodeA to nodeB. If the links connects from nodeB to nodeA, the function will
 * return false.
 *
 * @param nodeA Node at end A of link.
 * @param nodeB Node at end B of link.
 * @return True if nodeA is connected to nodeB, false otherwise.
 */
bool Link::isNodeAlinkedToNodeB(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB) const {
	return (this->nodeA == nodeA && this->nodeB == nodeB);
}