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

#include "LinkTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
LinkTest::LinkTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "LinkTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
}

/// Construct Link, test default values.
TEST_F(LinkTest, Constructor) {
	std::shared_ptr<Node> source(std::make_shared<Node>(simulatorGlobals));
	std::shared_ptr<Node> destination(std::make_shared<Node>(simulatorGlobals));
	double bandwidth = 100000000; // 100 Mibits/s.
	double propagationDelay = 0.010; // 10ms delay.
	std::string name = "SimplexLink source->destination";
	Link linkWithName(source, destination, bandwidth, propagationDelay, simulatorGlobals, scheduler, name);
	Link linkNoName(source, destination, bandwidth, propagationDelay, simulatorGlobals, scheduler);

	EXPECT_EQ(source, linkWithName.getSourceNode());
	EXPECT_EQ(destination, linkWithName.getDestinationNode());
	EXPECT_EQ(bandwidth, linkWithName.getBandwidth());
	EXPECT_EQ(propagationDelay, linkWithName.getPropagationDelay());
	EXPECT_EQ(name, linkWithName.getName());
	EXPECT_EQ(std::numeric_limits<unsigned int>::max(), linkWithName.getTransmissionQueueSizeLimit());

	EXPECT_EQ(source, linkNoName.getSourceNode());
	EXPECT_EQ(destination, linkNoName.getDestinationNode());
	EXPECT_EQ(bandwidth, linkNoName.getBandwidth());
	EXPECT_EQ(propagationDelay, linkNoName.getPropagationDelay());
	EXPECT_EQ("", linkNoName.getName());
	EXPECT_EQ(std::numeric_limits<unsigned int>::max(), linkNoName.getTransmissionQueueSizeLimit());
}

/// Transmit/Propagate PDU test.
TEST_F(LinkTest, TransmitAndPropagate) {
	std::shared_ptr<Node> source(std::make_shared<Node>(simulatorGlobals));
	std::shared_ptr<Node> destination(std::make_shared<Node>(simulatorGlobals));
	double bandwidth = 100000000; // 100 MiB.
	double propagationDelay = 0.010; // 10ms delay.
	std::string name = "SimplexLink source->destination";
	// Creates a PDU, size 1000 bytes.
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(simulatorGlobals, 1, nullptr, source, destination, 1000));
	// Create simplex link.
	Link link(source, destination, bandwidth, propagationDelay, simulatorGlobals, scheduler, name);
	// Set previous and next to prevent rejection of PDU by link.
	pdu->previous = pdu->source;
	pdu->next = pdu->destination;
	EXPECT_EQ(pdu->previous, pdu->source);
	EXPECT_EQ(pdu->next, pdu->destination);

	// Phase 1: initiate transmission.
	EXPECT_EQ(link.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pdu), LinkReturnType::PDU_IN_TRANSMISSION_NEXT_EVENT_SCHEDULED);
	// Test event that was scheduled and its parameters.
	Event event = scheduler.cause();
	EXPECT_EQ(event.eventType, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK);
	EXPECT_EQ(event.occurAfterTime, 1000 * 8 / bandwidth);
	// Check clock.
	EXPECT_EQ(simulatorGlobals.getCurrentAbsoluteTime(), 1000 * 8 / bandwidth);
	// Is link busy? It must be.
	EXPECT_EQ(true, link.isTransmissionServerBusy());
	// Check link transmission and inTransitQueue.
	EXPECT_EQ(0, link.getTransmissionQueueSize()); // Did not wait in queue.
	EXPECT_EQ(0, link.getInTransitQueueSize()); // Not propagating yet.
	
	// Phase 2: now propagate PDU.
	EXPECT_EQ(LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED, link.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pdu));
	// Check inTransitQueue.
	EXPECT_EQ(1, link.getInTransitQueueSize());
	// Link's transmission server must be available now, since the propagation event releases the underlying Facility.
	EXPECT_EQ(false, link.isTransmissionServerBusy());
	// Test event that was scheduled and its parameters.
	event = scheduler.cause();
	EXPECT_EQ(event.eventType, EventType::PDUTOKEN_ARRIVAL_AT_NODE);
	EXPECT_EQ(event.occurAfterTime, propagationDelay);
	// Check clock.
	EXPECT_EQ(simulatorGlobals.getCurrentAbsoluteTime(), 1000 * 8 / bandwidth + propagationDelay);
	
	// Phase 3: link delivers PDU, node receives PDU, PDU is fully propagated.
	link.endPropagation(pdu); // Ends propagation, removes PDU from inTransitQueue.
	EXPECT_EQ(0, link.getInTransitQueueSize());
	EXPECT_EQ(0, scheduler.getChainSize()); // No more events in event chain.
	EXPECT_EQ(0, link.getDroppedPdusCountWholeLink());

	// Now let's change the same PDU's previous and next Node fields to some invalid ones and check whether a link accepts transmitting a PDU that will not reach its next node through it.
	std::shared_ptr<Node> notDestination(std::make_shared<Node>(simulatorGlobals));
	pdu->previous = pdu->source;
	pdu->next = notDestination;
	// Deliver it to the Link. It should refuse it.
	EXPECT_EQ(link.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pdu), LinkReturnType::ERROR_LINK_DOES_NOT_CONNECT_NODES);
}

/**
 * @brief Transmit/Propagate several PDUs, test large BDP link, and queues.
 *
 * @details
 * Topology:
 *
 * Node0 --------- Node1 ---------- Node2
 *                  |
 *                  |
 *                  |
 *                 Node3
 *
 * PDUs get inserted into Node0. Some will go to Node2 (routeToNode2), some will go to Node3 (routetoNode3).
 *
 */
TEST_F(LinkTest, MultiplePdus) {
	// Some variables.
	Event event;
	std::shared_ptr<Entity> eventEntityNonConst; // To temporarily use for removal of constness of Entity within event.
	std::shared_ptr<ProtocolDataUnit> pduFromEventEntityNonConst;
	bool simulationEnded = false;
	unsigned int pduSize = 1000; // PDU will be 1000 bytes.

	
	// Build 4 nodes.
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> routeToNode2;
	std::vector<std::shared_ptr<Entity>> routeToNode3;
	for (int i = 0; i < 4; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Create route from node0 to node2.
	routeToNode2.push_back(nodeVector.at(0));
	routeToNode2.push_back(nodeVector.at(1));
	routeToNode2.push_back(nodeVector.at(2));
	
	// Create route from node0 to node3.
	routeToNode3.push_back(nodeVector.at(0));
	routeToNode3.push_back(nodeVector.at(1));
	routeToNode3.push_back(nodeVector.at(3));

	// Create simplex links between nodes.
	Link link01(nodeVector.at(0), nodeVector.at(1), 100000, 0.001, simulatorGlobals, scheduler); // 100 Kbps, 1ms.
	link01.setTransmissionQueueSizeLimit(9); // PDUs above 10 will be discarded.
	Link link12(nodeVector.at(1), nodeVector.at(2), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1s, 3 PDU queue limit.
	link12.setTransmissionQueueSizeLimit(3);
	Link link13(nodeVector.at(1), nodeVector.at(3), 100000, 10.0, simulatorGlobals, scheduler); // 100 Kbps, 10s, 3 PDU queue limit.
	link13.setTransmissionQueueSizeLimit(3);

	// The goal here is to also verify whether links are dropping PDUs because of full queues. Links after Node 1 will actually not have full queues with the current configuration,
	// since not all PDUs will arrive at the same time at Node 1 due to the delay imposed by link01. Actually, the queue at link01 will have PDUs waiting.

	// Generate 5 PDUs to be sent them to node2 with corresponding events.
	for (int i = 0; i < 5; ++i) {
		scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
			nodeVector.at(0), nodeVector.at(2), nodeVector.at(0), nodeVector.at(0), pduSize, routeToNode2))));
	}
	// Generate 5 PDUs to be sent them to node3 with corresponding events. Arrival time will be 1ms later than previous 5 PDUs.
	for (int i = 0; i < 5; ++i) {
		scheduler.schedule(Event(0.001, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
			nodeVector.at(0), nodeVector.at(3), nodeVector.at(0), nodeVector.at(0), pduSize, routeToNode3))));
	}

	// Generate 5 additional PDUs to be sent do node3 with corresponding events. These should be actually dropped by link01, because the queue will be already full.
	for (int i = 0; i < 5; ++i) {
		scheduler.schedule(Event(0.002, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
			nodeVector.at(0), nodeVector.at(3), nodeVector.at(0), nodeVector.at(0), pduSize, routeToNode3))));
	}

	EXPECT_EQ(15, scheduler.getChainSize());
	// Schedule one END_SIMULATION event.
	scheduler.schedule(Event(1000, EventType::END_SIMULATION, nullptr));

	// Main switch-case.
	while (!simulationEnded) {
		event = scheduler.cause(); // Fetch next event from scheduler.
		eventEntityNonConst = std::const_pointer_cast<Entity>(event.entity);
		pduFromEventEntityNonConst = std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst);
		// Now decide what to do.
		switch (event.eventType) {
			//case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
			//	// Decide which link to use based on next field of PDU.
			//	// Return type of Link depends on whether the link is busy or whether queue has reached maximum.
			//	if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // All PDUs.
			//		if (pduFromEventEntityNonConst->id == 1) {
			//			EXPECT_EQ(link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSMISSION_NEXT_EVENT_SCHEDULED);
			//		} else {
			//			EXPECT_EQ(link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_PDU_ENQUEUED);
			//		}
			//	} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // PDUs 1 - 5
			//		if (pduFromEventEntityNonConst->id == 1) {
			//			EXPECT_EQ(link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSMISSION_NEXT_EVENT_SCHEDULED);
			//		} else if (pduFromEventEntityNonConst->id >= 2 && pduFromEventEntityNonConst->id <= 3) {
			//			EXPECT_EQ(link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_PDU_ENQUEUED);
			//		} else {
			//			EXPECT_EQ(link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED);
			//		}
			//	} else {
			//		if (pduFromEventEntityNonConst->id == 6) { // PDUs 6 - 10.
			//			EXPECT_EQ(link13.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSMISSION_NEXT_EVENT_SCHEDULED);
			//		} else if (pduFromEventEntityNonConst->id >= 7 && pduFromEventEntityNonConst->id <= 8) {
			//			EXPECT_EQ(link13.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_PDU_ENQUEUED);
			//		} else {
			//			EXPECT_EQ(link13.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED);
			//		}
			//	}
			//	break;

			case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
				// Decide which link to use based on next field of PDU.
				// Return type of Link depends on whether the link is busy or whether queue has reached maximum.
				// Node 1, link 01.
				if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // All PDUs.
					// PDUs above 11 should be discarded by link01.
					if (pduFromEventEntityNonConst->id >= 11 && pduFromEventEntityNonConst->id <= 15) {
						EXPECT_EQ(LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED, link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst));
					} else {
						// Other PDUs will request this event either by first arriving at node1, or when being dequeued.
						link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
					}
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // PDUs 1 - 5
					// Node 1, link 12.
					if (pduFromEventEntityNonConst->id >=1 && pduFromEventEntityNonConst->id <= 5) {
						link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
					} else {
						EXPECT_EQ(link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED);
					}
				} else {
					// Node 1, link 13.
					// PDUs 6 - 10.
					if (pduFromEventEntityNonConst->id >= 6 && pduFromEventEntityNonConst->id <= 10) {
						link13.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
					} else {
						EXPECT_EQ(link13.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED);
					}
				}
				break;

			case EventType::PDUTOKEN_ARRIVAL_AT_NODE:
				// Test for arrival at each node and perform tests.
				if (pduFromEventEntityNonConst->next == nodeVector.at(0)) { // Node 0.
					// PDUs from 1 to 5 should arrive at 0.0.
					if (pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 5) {
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), 0.0);
						EXPECT_DOUBLE_EQ(0.0, event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(2), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					} else if (pduFromEventEntityNonConst->id >= 6 && pduFromEventEntityNonConst->id <= 10) {
						// PDUs from 6 to 10 should arrive at 0.001.
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), 0.001);
						EXPECT_DOUBLE_EQ(0.001, event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(3), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					} else {
						// PDUs from 11 to 15 should arrive at 0.002.
						EXPECT_DOUBLE_EQ(0.002, simulatorGlobals.getCurrentAbsoluteTime());
						EXPECT_DOUBLE_EQ(0.002, event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(3), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					}
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // Node 1.
					// End propagation.
					link01.endPropagation(pduFromEventEntityNonConst);
					// PDUs from 1 to 5 should arrive at pduSize * 8 / bandwidthlink01 + propagationDelayLink01 + queue wait time previous PDUs.
					if (pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 5) {
						// Each subsequent PDU adds 0.08 of delay, since a PDU has to wait in the queue while to previous PDUs are being transmitted.
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), (pduSize * 8 / link01.getBandwidth()) * (pduFromEventEntityNonConst->id) + link01.getPropagationDelay());
						EXPECT_DOUBLE_EQ(link01.getPropagationDelay(), event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(2), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					} else {
						// PDUs from 6 to 10 should arrive at pduSize * 8 / bandwidthlink01 + propagationDelayLink01 + queue wait time previous PDUs.
						// Arrival delay of PDUs 6-10 will not accumulate, since when these PDUs arrive, they will go directly to the queue.
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), pduSize * 8 / link01.getBandwidth() * pduFromEventEntityNonConst->id + link01.getPropagationDelay());
						EXPECT_DOUBLE_EQ(link01.getPropagationDelay(), event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(3), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					}
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // Node 2.
					// End propagation.
					link12.endPropagation(pduFromEventEntityNonConst);
					// PDUs from 1 to 3 should arrive at pduSize / bandwidthlink12 + propagationDelayLink12 + previous link latency + queue wait time previous PDUs at link01.
					// PDUs from 4 to 5 should be discarded. Actually, NO!
					if (pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 5) {
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), pduSize * 8 / link12.getBandwidth() + link12.getPropagationDelay() + pduSize * 8 / link01.getBandwidth() * pduFromEventEntityNonConst->id + link01.getPropagationDelay());
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), pduFromEventEntityNonConst->getAbsoluteGenerationTime() + pduSize * 8 / link12.getBandwidth() + link12.getPropagationDelay() + pduSize * 8 / link01.getBandwidth() * pduFromEventEntityNonConst->id + link01.getPropagationDelay());
						EXPECT_DOUBLE_EQ(link12.getPropagationDelay(), event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(2), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					} else {
						// No other PDUs should be here.
						EXPECT_TRUE(pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 5);
					}
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 3.
					// End propagation.
					link13.endPropagation(pduFromEventEntityNonConst);
					// PDUs from 6 to 8 should arrive at pduSize / bandwidthlink13 + propagationDelayLink13 + previous link latency + queue wait time previous PDUs at link01.
					// PDUs from 9 to 10 should be discarded. Actually, NO!
					if (pduFromEventEntityNonConst->id >= 6 && pduFromEventEntityNonConst->id <= 10) {
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), pduSize * 8 / link13.getBandwidth() + link13.getPropagationDelay() + pduSize * 8 / link01.getBandwidth() * pduFromEventEntityNonConst->id + link01.getPropagationDelay());
						EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), pduFromEventEntityNonConst->getAbsoluteGenerationTime() + pduSize * 8 / link13.getBandwidth() + link13.getPropagationDelay() + pduSize * 8 / link01.getBandwidth() * pduFromEventEntityNonConst->id + link01.getPropagationDelay());
						EXPECT_DOUBLE_EQ(link13.getPropagationDelay(), event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(3), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					} else {
						// No other PDUs should be here.
						EXPECT_TRUE(pduFromEventEntityNonConst->id >= 6 && pduFromEventEntityNonConst->id <= 10);
					}
				}
				// This should be the same for either route. The "next" node should to the processing; it is actually the "current" node at this stage/event.
				if (pduFromEventEntityNonConst->next != nodeVector.at(2) && pduFromEventEntityNonConst->next != nodeVector.at(3)) {
					// Not Final destination; forward it.
					EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
					scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				} else {
					// Final destination. Do not schedule anything else.
					EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
				}
				// Schedule the transmission.
				//scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				//EXPECT_EQ(link01.getDestinationNode(), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next));
				break;

			case EventType::END_SIMULATION:
				std::cout << "Simulation ended." << std::endl;
				simulationEnded = true;
				break;

			case EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK:
				// Here, end the link transmission and schedule the end of propagation.
				if (pduFromEventEntityNonConst->next == nodeVector.at(1)) {
					EXPECT_EQ(link01.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) {
					EXPECT_EQ(link12.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else {
					EXPECT_EQ(link13.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				}
				break;

			default:
				EXPECT_FALSE(true); // This should never be reached.
				break;
		}
	} // End while.

	// Test statistics.
	// Nodes.
	EXPECT_EQ(pduSize * 15, nodeVector.at(0)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 15, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(pduSize * 10, nodeVector.at(1)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 10, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 5, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(3)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 5, nodeVector.at(3)->getReceivedBytesCount());

	// Links.
	// Links 12 and 13 will never drop any PDU, because they will actually have 0 PDUs in queue at any time...
	EXPECT_EQ(0, link01.getDroppedPdusCountMedium());
	EXPECT_EQ(5, link01.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(5, link01.getDroppedPdusCountWholeLink());
	EXPECT_EQ(0, link12.getDroppedPdusCountMedium());
	EXPECT_EQ(0, link12.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(0, link12.getDroppedPdusCountWholeLink());
	EXPECT_EQ(0, link13.getDroppedPdusCountMedium());
	EXPECT_EQ(0, link13.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(0, link13.getDroppedPdusCountWholeLink());

	// The tests below demonstrate how link01 had 9 PDUs in queue, and the other links never got any queue, since each PDU arrived after the previous one was already departed.
	EXPECT_EQ(9, link01.getMaxRecordedTransmissionQueueSize());
	EXPECT_EQ(0, link12.getMaxRecordedTransmissionQueueSize());
	EXPECT_EQ(0, link13.getMaxRecordedTransmissionQueueSize());
}

/**
* @brief Transmit/Propagate several PDUs, test large BDP link, and queues.
*
* @details 
* Topology:
*
* Node0 --------- Node1 ---------- Node2
*
*/
TEST_F(LinkTest, MultiplePdusSimpleTopology) {
	// Some variables.
	Event event;
	std::shared_ptr<Entity> eventEntityNonConst; // To temporarily use for removal of constness of Entity within event.
	std::shared_ptr<ProtocolDataUnit> pduFromEventEntityNonConst;
	bool simulationEnded = false;
	unsigned int pduSize = 1000; // PDU will be 1000 bytes.

	// Build 3 nodes.
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> routeToNode2;
	for (int i = 0; i < 3; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Create route from node0 to node2.
	routeToNode2.push_back(nodeVector.at(0));
	routeToNode2.push_back(nodeVector.at(1));
	routeToNode2.push_back(nodeVector.at(2));

	// Create simplex links between nodes.
	Link link01(nodeVector.at(0), nodeVector.at(1), 100000000, 0.0001, simulatorGlobals, scheduler); // 100 Mbps, 0.1ms. Very fast.
	//link01.setTransmissionQueueSizeLimit(9); // PDUs above 10 will be discarded.
	Link link12(nodeVector.at(1), nodeVector.at(2), 10000, 1.0, simulatorGlobals, scheduler); // 10 Kibps, 1s, 3 PDU queue limit. Very slow.
	link12.setTransmissionQueueSizeLimit(2); // Hold only two PDUs in queue.

	// Generate 6 PDUs to be sent them to node2 with corresponding events.
	for (int i = 0; i < 6; ++i) {
		scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
			nodeVector.at(0), nodeVector.at(2), nodeVector.at(0), nodeVector.at(0), pduSize, routeToNode2))));
	}

	EXPECT_EQ(6, scheduler.getChainSize());
	// Schedule one END_SIMULATION event.
	scheduler.schedule(Event(1000, EventType::END_SIMULATION, nullptr));

	// Main switch-case.
	while (!simulationEnded) {
		event = scheduler.cause(); // Fetch next event from scheduler.
		eventEntityNonConst = std::const_pointer_cast<Entity>(event.entity);
		pduFromEventEntityNonConst = std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst);
		// Now decide what to do.
		switch (event.eventType) {
			case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
				// Decide which link to use based on next field of PDU.
				// Return type of Link depends on whether the link is busy or whether queue has reached maximum.
				// Node 1, link 01.
				if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // All PDUs.
					// PDUs will request this event either by first arriving at node1, or when being dequeued.
					link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) {
					// Node 1, link 12.
					// PDUs 1 to 3 should be fine (one in transmission, two in queue). Other whould be discarded.
					if (pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 3) {
						// PDUs 1 to 3 should request this event both by arriving at the link, or by being dequeued from transmission server.
						link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
					} else {
						EXPECT_EQ(link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst), LinkReturnType::LINK_BUSY_QUEUE_FULL_PDU_DROPPED);
					}
				}
				break;

			case EventType::PDUTOKEN_ARRIVAL_AT_NODE:
				// Test for arrival at each node and perform tests.
				if (pduFromEventEntityNonConst->next == nodeVector.at(0)) { // Node 0.
					// All PDUs should arrive at 0.0.
					EXPECT_DOUBLE_EQ(simulatorGlobals.getCurrentAbsoluteTime(), 0.0);
					EXPECT_DOUBLE_EQ(0.0, event.occurAfterTime);
					EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
					EXPECT_EQ(nodeVector.at(2), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // Node 1.
					// End propagation.
					link01.endPropagation(pduFromEventEntityNonConst);
					// All PDUs should arrive at pduSize * 8 / bandwidthlink01 + propagationDelayLink01 + queue wait time previous PDUs.
					// Each subsequent PDU adds 0.08 of delay, since a PDU has to wait in the queue while to previous PDUs are being transmitted.
					EXPECT_DOUBLE_EQ((pduSize * 8 / link01.getBandwidth()) * (pduFromEventEntityNonConst->id) + link01.getPropagationDelay(), simulatorGlobals.getCurrentAbsoluteTime());
					EXPECT_DOUBLE_EQ(event.occurAfterTime, link01.getPropagationDelay());
					EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
					EXPECT_EQ(nodeVector.at(2), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // Node 2.
					// End propagation.
					link12.endPropagation(pduFromEventEntityNonConst);
					// PDUs from 1 to 3 should arrive at pduSize / bandwidthlink12 + propagationDelayLink12 + previous link latency + queue wait time previous PDUs at link01.
					// PDUs from 4 to 6 should be discarded when they requested transmission at link12.
					// Check file SimplexLinkTextMultiplePdusSimpleTopology.xlsx for these calculations.
					if (pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 3) {
						switch (pduFromEventEntityNonConst->id) {
							case 1:
								EXPECT_DOUBLE_EQ(1.80018, simulatorGlobals.getCurrentAbsoluteTime());
								break;
							case 2:
								EXPECT_DOUBLE_EQ(2.60018, simulatorGlobals.getCurrentAbsoluteTime()); 
								break;
							case 3:
								EXPECT_DOUBLE_EQ(3.40018, simulatorGlobals.getCurrentAbsoluteTime()); 
								break;
							default:
								break;
						}
						
						EXPECT_DOUBLE_EQ(link12.getPropagationDelay(), event.occurAfterTime);
						EXPECT_EQ(nodeVector.at(0), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->source));
						EXPECT_EQ(nodeVector.at(2), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->destination));
					} else {
						// No other PDUs should be here.
						EXPECT_TRUE(pduFromEventEntityNonConst->id >= 4 && pduFromEventEntityNonConst->id <= 6);
					}
				}
				// This should be the same for either route. The "next" node should to the processing; it is actually the "current" node at this stage/event.
				if (pduFromEventEntityNonConst->next != nodeVector.at(2)) {
					// Not Final destination; forward it.
					EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
					scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				} else {
					// Final destination. Do not schedule anything else.
					EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
				}
				break;

			case EventType::END_SIMULATION:
				std::cout << "Simulation ended." << std::endl;
				simulationEnded = true;
				break;

			case EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK:
				// Here, end the link transmission and schedule the end of propagation.
				if (pduFromEventEntityNonConst->next == nodeVector.at(1)) {
					EXPECT_EQ(link01.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) {
					EXPECT_EQ(link12.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else {
					EXPECT_FALSE(true); // Should not happen. Mark as error.
				}
				break;

			default:
				EXPECT_FALSE(true); // This should never be reached.
				break;
		} // End switch.
	} // End while.

	// Test statistics.
	// Nodes.
	EXPECT_EQ(pduSize * 6, nodeVector.at(0)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 6, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(pduSize * 6, nodeVector.at(1)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 6, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 3, nodeVector.at(2)->getReceivedBytesCount());

	// Links.
	// Links 12 should drop 3 PDUs.
	EXPECT_EQ(0, link01.getDroppedPdusCountMedium());
	EXPECT_EQ(0, link01.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(0, link01.getDroppedPdusCountWholeLink());
	EXPECT_EQ(0, link12.getDroppedPdusCountMedium());
	EXPECT_EQ(3, link12.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(3, link12.getDroppedPdusCountWholeLink());

	// The tests below demonstrate how link01 had 9 PDUs in queue, and the other links never got any queue, since each PDU arrived after the previous one was already departed.
	EXPECT_EQ(5, link01.getMaxRecordedTransmissionQueueSize());
	EXPECT_EQ(2, link12.getMaxRecordedTransmissionQueueSize());
}


/**
* @brief Transmit/Propagate several PDUs, set link down, test dropped PDUs.
*
* @details 
* Topology:
*
* Node0 --------- Node1
*
*/
TEST_F(LinkTest, MultiplePdusMediumDrop) {
	// Some variables.
	Event event;
	std::shared_ptr<Entity> eventEntityNonConst; // To temporarily use for removal of constness of Entity within event.
	std::shared_ptr<ProtocolDataUnit> pduFromEventEntityNonConst;
	bool simulationEnded = false;
	unsigned int pduSize = 1000; // PDU will be 1000 bytes.

	// Build 2 nodes.
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> routeToNode1;
	for (int i = 0; i < 2; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Create route from node0 to node1.
	routeToNode1.push_back(nodeVector.at(0));
	routeToNode1.push_back(nodeVector.at(1));
	
	// Create simplex links between nodes.
	Link link01(nodeVector.at(0), nodeVector.at(1), 8000, 2, simulatorGlobals, scheduler); // 8 Kbps, 2s. Transmission takes 1s per PDU, very slow propagation, to force dropping these PDUs in transit.
	
	// Generate 6 PDUs to be sent them to node1 with corresponding events.
	for (int i = 0; i < 6; ++i) {
		scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
			nodeVector.at(0), nodeVector.at(1), nodeVector.at(0), nodeVector.at(0), pduSize, routeToNode1))));
	}
		
	// Schedule one END_SIMULATION event.
	scheduler.schedule(Event(1000, EventType::END_SIMULATION, nullptr));
	// Schedule one LINK DOWN event.
	scheduler.schedule(Event(3.5, EventType::SET_LINK_DOWN, nullptr));
	EXPECT_EQ(8, scheduler.getChainSize());

	// Main switch-case.
	while (!simulationEnded) {
		event = scheduler.cause(); // Fetch next event from scheduler.
		eventEntityNonConst = std::const_pointer_cast<Entity>(event.entity);
		pduFromEventEntityNonConst = std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst);
		// Now decide what to do.
		switch (event.eventType) {
			case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
				// Node 1, link 01.
				link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				break;

			case EventType::PDUTOKEN_ARRIVAL_AT_NODE:
				// Test for arrival. All PDUs arrive at node 0.
				if (pduFromEventEntityNonConst->next == nodeVector.at(0)) { // Node 0.
					EXPECT_TRUE(pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 6);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // Node 1.
					// Only PDU 1 should arrive at node 1; others will be dropped in transit and in link transmission.
					EXPECT_TRUE(pduFromEventEntityNonConst->id == 1);
				}
				// Perform forwarding.
				if (pduFromEventEntityNonConst->next != nodeVector.at(1)) {
					// Not Final destination; forward it.
					EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
					scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				} else {
					// Final destination. Do not schedule anything else.
					// End propagation.
					link01.endPropagation(pduFromEventEntityNonConst);
					EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
				}
				break;

			case EventType::END_SIMULATION:
				std::cout << "Simulation ended." << std::endl;
				simulationEnded = true;
				break;

			case EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK:
				// Here, end the link transmission and schedule the end of propagation.
				if (pduFromEventEntityNonConst->id >= 1 && pduFromEventEntityNonConst->id <= 4) {
					EXPECT_EQ(LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED, link01.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst));
				} else {
					EXPECT_EQ(LinkReturnType::LINK_DOWN_PDU_DROPPED, link01.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst));
				}
				break;

			case EventType::SET_LINK_DOWN:
				link01.setDown();
				EXPECT_FALSE(link01.isUp());
				break;

			default:
				EXPECT_FALSE(true); // This should never be reached.
				break;
		} // End switch.
	} // End while.

	// Test statistics.
	// Nodes.
	EXPECT_EQ(pduSize * 6, nodeVector.at(0)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 6, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(1)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 1, nodeVector.at(1)->getReceivedBytesCount());
	
	// Links.
	// Links 01 should drop 3 PDUs.
	EXPECT_EQ(2, link01.getDroppedPdusCountMedium());
	EXPECT_EQ(3, link01.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(5, link01.getDroppedPdusCountWholeLink());
	
	// The tests below demonstrate how link01 had 5 PDUs in queue.
	EXPECT_EQ(5, link01.getMaxRecordedTransmissionQueueSize());
}

/**
* @brief Transmit/Propagate several PDUs back and forth in a duplex link. Link fault.
*
* @details 
* Topology:
*
* Node0 --------- Node1
*
*/
TEST_F(LinkTest, DuplexLinkAndDrop) {
	// Some variables.
	Event event;
	std::shared_ptr<Entity> eventEntityNonConst(nullptr); // To temporarily use for removal of constness of Entity within event.
	std::shared_ptr<ProtocolDataUnit> pduFromEventEntityNonConst(nullptr);
	bool simulationEnded = false;
	unsigned int pduSize = 1000; // PDU will be 1000 bytes.
	unsigned int ackPduSize = 1; // Ack PDU will be 1 byte.
	NodeReturnType nodeReturnType;
	
	// Build 2 nodes.
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> route01;
	std::vector<std::shared_ptr<Entity>> route10;
	for (int i = 0; i < 2; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Create route from node0 to node1.
	route01.push_back(nodeVector.at(0));
	route01.push_back(nodeVector.at(1));

	// Create route from node1 to node0.
	route10.push_back(nodeVector.at(1));
	route10.push_back(nodeVector.at(0));

	// Create duplex link between nodes.
	Link link(nodeVector.at(0), nodeVector.at(1), 8000, 2, simulatorGlobals, scheduler, "", LinkType::DUPLEX_LINK); // 8 Kbps, 2s. Transmission takes 1s per PDU, very slow propagation.
	
	// Generate 6 PDUs to be sent them to node1 with corresponding events.
	for (int i = 0; i < 6; ++i) {
		scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
			nodeVector.at(0), nodeVector.at(1), nodeVector.at(0), nodeVector.at(0), pduSize, route01))));
	}
		
	// Schedule one END_SIMULATION event.
	scheduler.schedule(Event(1000, EventType::END_SIMULATION, nullptr));
	// Schedule one LINK DOWN event.
	scheduler.schedule(Event(3.5, EventType::SET_LINK_DOWN, nullptr));
	EXPECT_EQ(8, scheduler.getChainSize());

	// Main switch-case.
	while (!simulationEnded) {
		event = scheduler.cause(); // Fetch next event from scheduler.
		eventEntityNonConst = std::const_pointer_cast<Entity>(event.entity);
		pduFromEventEntityNonConst = std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst);
		// Now decide what to do.
		switch (event.eventType) {
			case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
				// Transmit PDU; direction depends on PDU.
				link.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				break;

			case EventType::PDUTOKEN_ARRIVAL_AT_NODE:
				// Test link direction and behavior of functions that determine link direction.
				// Also verify whether the PDU has just being created; in that case, this is the source node, and "previous" and "next" will be equal to the source node until the PDU is processed/forwarded.
				if (pduFromEventEntityNonConst->previous != pduFromEventEntityNonConst->next) { // Not source node.	
					if (std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->previous) == nodeVector.at(0)) {
						// Direction node0 to node1. Test.
						EXPECT_TRUE((link.isNodeAlinkedToNodeB(std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->previous), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next))));
					} else if (std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->previous) == nodeVector.at(1)) {
						EXPECT_TRUE((link.getReverseLink()->isNodeAlinkedToNodeB(std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->previous), std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next))));
					} else {
						EXPECT_FALSE(true); // Fail here.
					}
				} else {
					// This is the source node. Do nothing.
					EXPECT_TRUE(true);
				}
				// End propagation at proper link. Note that if this is the source node, endPropagation will do nothing, since the PDU is not yet at the link.
				// Note, additionally, that endPropagation has intelligence to determine the proper link direction given the PDU.
				link.endPropagation(pduFromEventEntityNonConst);
				
				// Perform forwarding. This should be done "after" ending propagation at current link, thus meaning the PDU has "left" the link.
				nodeReturnType = std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst));
				if (nodeReturnType == NodeReturnType::PDU_ROUTE_UPDATED) {
					// Not final destination; schedule transmission.
					//link.endPropagation(pduFromEventEntityNonConst);
					scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				} else if (nodeReturnType == NodeReturnType::FINAL_DESTINATION) {
					// Final destination. Now check whether we must generate an ACK PDU.
					if (std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next) == nodeVector.at(1)) {
						// Arrived at node1; generate an ACK towards node0.
						scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, std::shared_ptr<ProtocolDataUnit>(new ProtocolDataUnit(simulatorGlobals, 1, nullptr,
							nodeVector.at(1), nodeVector.at(0), nodeVector.at(1), nodeVector.at(1), ackPduSize, route10))));
					}
				}
				break;

			case EventType::END_SIMULATION:
				std::cout << "Simulation ended." << std::endl;
				simulationEnded = true;
				break;

			case EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK:
				// Here, end the link transmission and schedule the end of propagation.
				link.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst);
				break;

			case EventType::SET_LINK_DOWN:
				EXPECT_EQ(6, link.setDown()); // 5 PDUs in node0 queue, + 1 ACK in transmission at node1.
				EXPECT_FALSE(link.isUp());
				break;

			default:
				EXPECT_FALSE(true); // This should never be reached.
				break;
		} // End switch.
	} // End while.

	// Test statistics.
	// Nodes.
	EXPECT_EQ(pduSize * 6, nodeVector.at(0)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 6, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(ackPduSize * 1, nodeVector.at(1)->getForwardedBytesCount());
	EXPECT_EQ(pduSize * 1 + ackPduSize * 1, nodeVector.at(1)->getReceivedBytesCount());
	
	// Links.
	// Links 01 should drop 3 PDUs.
	EXPECT_EQ(2, link.getDroppedPdusCountMedium());
	EXPECT_EQ(3, link.getDroppedPdusCountTransmissionServer());
	EXPECT_EQ(5, link.getDroppedPdusCountWholeLink());
	EXPECT_EQ(1, link.getReverseLink()->getDroppedPdusCountMedium());
	EXPECT_EQ(0, link.getReverseLink()->getDroppedPdusCountTransmissionServer()); // Transmission of ACK1 takes 0.001s, finishes at 3.001s. Propagation would end at 5.001s.
	EXPECT_EQ(1, link.getReverseLink()->getDroppedPdusCountWholeLink()); // Node1 only receives one PDU, PDU1. PDU2 would reach node1 at 4s.
	
	// The tests below demonstrate how link01 had 5 PDUs in queue.
	EXPECT_EQ(5, link.getMaxRecordedTransmissionQueueSize());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}