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

#pragma once

#include "Entity.h"
#include "Node.h"
#include "ProtocolDataUnit.h"
#include "SimulatorGlobals.h"
#include "Scheduler.h"
#include "Facility.h"
#include "LinkReturnType.h"
#include "LinkType.h"
#include <string>
#include <list>

/**
 * @brief Link class.
 * 
 * @par Description
 * A Link connects two Node objects.
 *
 * Typically, a Link can be simplex (one-way) or duplex (two-way). A duplex link is basically two simplex links, one opposite to the other in node direction; the other
 * parameters are common.
 *
 * Internally, a link has a Facility object with one server. The facility transmits the PDU with speed proportional to the bandwidth, i.e.,
 * transmission time = packet size / bandwidth. After transmission, the PDU is propagated through the link, with time delay = propagation delay or latency.
 * The propagation delay is medium-dependent. Fiber optics is a faster medium than copper. Propagation delay is proportional to the
 * distance between the nodes and the speed of light (or electrons) in the medium. Propagation delay is a fixed time delay, defined when creating the link, and models
 * an infinite capacity server from Queuing Theory.
 *
 * Conceptually, a link will forward a PDU in two stages, with an event for each: transmission, and propagation. If a link goes down, all PDUs currently in transmission queue
 * and in propagation queue or transit queue. Hence, there are two queues here: one from the transmission Facility, and one from the propagation delay center.
 * Naturally, if a link has a high bandwidth-delay-product, there can be many PDUs in transit within the link at the same time. 
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
 *
 * The functions that return statistics, such as dropped PDUs at medium or at transmission server, are constrained to one link direction only. Thus, to obtain the total
 * statistics for both directions of a link, call the statistics functions for each direction and then add the results. The statistics will never overlap (i.e., one PDU
 * dropped in one direction will never be added to statistics to the other direction).
 */
class Link: public Entity {
private:
	std::shared_ptr<Node> nodeA; //!< Node at one side of the link.
	std::shared_ptr<Node> nodeB; //!< Node at the other side of the link.
	double bandwidth; //!< Bandwidth of this link, typically in bits per second.
	double propagationDelay; //!< Propagation delay of this link, which is medium-dependent, typically in seconds.
	std::string name; //!< Name of this link.
	std::list<std::shared_ptr<const ProtocolDataUnit>> inTransitQueue; //!< Queue for PDUs in transit (in propagation) in this link.
	SimulatorGlobals &simulatorGlobals; //!< Reference to SimulatorGlobals object.
	Scheduler &scheduler; //!< Reference to Scheduler object.
	Facility transmissionServer; //!< Transmission server for this Link (one-way).
	unsigned int droppedPdusCountMedium; //!< Count of dropped PDUs by this link's medium.
	LinkType linkType; //!< Type of link (typically Simplex or Duplex).
	std::shared_ptr<Link> reverseLink; //!< Contains the pointer for the link in the reverse direction in case of Duplex links.

	unsigned int purgeInTransitQueue();

public:
	Link(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB, double bandwidth, double propagationDelay,
		 SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, const std::string &name = "", LinkType linkType = LinkType::SIMPLEX_LINK);
	//virtual ~Link()=0; // Make this class pure virtual.

	virtual std::shared_ptr<Node> getNodeA() const;
	virtual std::shared_ptr<Node> getNodeB() const;
	virtual std::shared_ptr<Node> getSourceNode() const;
	virtual std::shared_ptr<Node> getDestinationNode() const;
	virtual double getBandwidth() const;
	virtual double getPropagationDelay() const;
	virtual std::string getName() const;
	virtual bool isUp() const;
	virtual LinkReturnType transmitPdu(EventType transmitEventType, EventType endTransmitEventType, std::shared_ptr<const ProtocolDataUnit> pdu); // Works for duplex links.
	virtual std::list<std::shared_ptr<const ProtocolDataUnit>>::size_type getInTransitQueueSize() const;
	virtual LinkReturnType propagatePdu(EventType nextEvent, std::shared_ptr<const ProtocolDataUnit> pdu); // Works for duplex links.
	virtual bool isTransmissionServerBusy() const;
	virtual LinkReturnType endPropagation(std::shared_ptr<ProtocolDataUnit> pdu); // Works for duplex links.
	virtual std::list<FacilityQueueElement>::size_type getTransmissionQueueSize() const;
	virtual unsigned int getMaxRecordedTransmissionQueueSize() const;
	virtual unsigned int getTransmissionQueueSizeLimit() const;
	virtual unsigned int getDroppedPdusCountTransmissionServer() const;
	virtual unsigned int getDroppedPdusCountMedium() const;
	virtual unsigned int getDroppedPdusCountWholeLink() const;
	virtual LinkType getLinkType() const;
	virtual std::shared_ptr<Link> getReverseLink() const;
	virtual bool isNodeAlinkedToNodeB(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB) const;

	// Setters.
	virtual void setUp(); // Works for duplex links.
	virtual unsigned int setDown(); // Works for duplex links.
	virtual void setTransmissionQueueSizeLimit(unsigned int limit); // Works for duplex links.

};
