/**
 * @author Marcos Portnoi
 * @date December 2013
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

#include "TrafficGeneratorAllRecordRouteTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
TrafficGeneratorAllRecordRouteTest::TrafficGeneratorAllRecordRouteTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "TrafficGeneratorAllRecordRouteTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
			simulatorGlobals.seedRandomNumberGenerator(1); // Use a fixed seed here for the tests.
}

/**
 * @brief Transmit/Propagate several PDUs, test large BDP link, and queues.
 *
 * @details 
 * Topology:
 *
 * Node0 --------- Node1 ---------- Node2
 *                  |    ---------- Node4
 *                  |    \--------- Node5
 *                  |     \-------- Node6
 *                  |
 *                 Node3
 *
 * PDUs get inserted into Node0. Some will go to Node2 (routeToNode2), some will go to Node3 (routetoNode3).
 *
 * Goal is to verify whether all traffic generators have correctly implemented the RecordRoute functions.
 * Each generator will direct PDUs to a unique route to an individual destination node. Verifying the recorded route will prove the correct RecordRoute implementation.
 */
TEST_F(TrafficGeneratorAllRecordRouteTest, RecordRoute) {
	// Some variables.
	Event event;
	std::shared_ptr<Entity> eventEntityNonConst; // To temporarily use for removal of constness of Entity within event.
	std::shared_ptr<ProtocolDataUnit> pduFromEventEntityNonConst;
	bool simulationEnded = false;
	unsigned int pduSize = 1000; // PDU will be 1000 bytes.
	
	// Build 7 nodes.
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> routeToNode2;
	std::vector<std::shared_ptr<Entity>> routeToNode3;
	std::vector<std::shared_ptr<Entity>> routeToNode4;
	std::vector<std::shared_ptr<Entity>> routeToNode5;
	std::vector<std::shared_ptr<Entity>> routeToNode6;
	for (int i = 0; i < 7; ++i) {
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

	// Create route from node0 to node4.
	routeToNode4.push_back(nodeVector.at(0));
	routeToNode4.push_back(nodeVector.at(1));
	routeToNode4.push_back(nodeVector.at(4));

	// Create route from node0 to node5.
	routeToNode5.push_back(nodeVector.at(0));
	routeToNode5.push_back(nodeVector.at(1));
	routeToNode5.push_back(nodeVector.at(5));
	
	// Create route from node0 to node6.
	routeToNode6.push_back(nodeVector.at(0));
	routeToNode6.push_back(nodeVector.at(1));
	routeToNode6.push_back(nodeVector.at(6));

	// Create simplex links between nodes.
	Link link01(nodeVector.at(0), nodeVector.at(1), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1ms.
	Link link12(nodeVector.at(1), nodeVector.at(2), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1s.
	Link link13(nodeVector.at(1), nodeVector.at(3), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1s.
	Link link14(nodeVector.at(1), nodeVector.at(4), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1s.
	Link link15(nodeVector.at(1), nodeVector.at(5), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1s.
	Link link16(nodeVector.at(1), nodeVector.at(6), 100000, 1.0, simulatorGlobals, scheduler); // 100 Kbps, 1s.
	
	// Create traffic generators.
	ConstantRateTrafficGenerator constantGenerator = ConstantRateTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, std::make_shared<Message>(Message("constant")),
																				  nodeVector.at(0), nodeVector.at(2), 1, 1);
	ExponentialTrafficGenerator expoGenerator = ExponentialTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, std::make_shared<Message>(Message("exponential")),
																			nodeVector.at(0), nodeVector.at(3), 1, 2.0);
	NormalTrafficGenerator normalGenerator = NormalTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, std::make_shared<Message>(Message("normal")),
																			nodeVector.at(0), nodeVector.at(4), 1, 2, .01);
	QcnSensorTrafficGenerator qcnGenerator = QcnSensorTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, std::make_shared<Message>(Message("qcn")),
																	   nodeVector.at(0), nodeVector.at(5), 1, 0, 0, 0, 0);
	WeibullTrafficGenerator weibullGenerator = WeibullTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, std::make_shared<Message>(Message("weibull")),
																	   nodeVector.at(0), nodeVector.at(6), 1, 1, 1);
	
	// It is important to turn on all traffic generators, otherwise nullptr exceptions might come up (due to null tokens when calling createInstance...).
	constantGenerator.turnOn();
	expoGenerator.turnOn();
	normalGenerator.turnOn();
	qcnGenerator.turnOn();
	weibullGenerator.turnOn();
	
	// Create traffic instances.
	constantGenerator.createInstanceTrafficEventPdu(pduSize, routeToNode2, true);
	expoGenerator.createInstanceTrafficEventPdu(pduSize, routeToNode3, true);
	normalGenerator.createInstanceTrafficEventPdu(pduSize, routeToNode4, true);
	qcnGenerator.createInstanceTrafficEventPdu(pduSize, routeToNode5, true);
	weibullGenerator.createInstanceTrafficEventPdu(pduSize, routeToNode6, true);
	
	// Schedule one END_SIMULATION event.
	scheduler.schedule(Event(1000, EventType::END_SIMULATION, nullptr));

	// Main switch-case.
	while (!simulationEnded) {
		event = scheduler.cause(); // Fetch next event from scheduler.
		eventEntityNonConst = std::const_pointer_cast<Entity>(event.entity);
		pduFromEventEntityNonConst = std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst);
		// Now decide what to do.
		switch (event.eventType) {
			case EventType::TRAFFIC_GENERATOR_ARRIVAL:
				// Arrival from traffic generator.
				// Just pass the PDU to the next event.
				scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst));
				break;

			case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
				// Decide which link to use based on next field of PDU.
				if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // All PDUs.
					link01.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // Constant generator only.
					link12.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Exponential generator only.
					link13.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(4)) { // Normal generator only.
					link14.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(5)) { // Qcn generator only.
					link15.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(6)) { // Weibull generator only.
					link16.transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				} else {
					EXPECT_FALSE(true); // Fail if it reaches here.
				}
				break;

			case EventType::PDUTOKEN_ARRIVAL_AT_NODE:
				// Test for arrival at each node and perform tests.
				if (pduFromEventEntityNonConst->next == nodeVector.at(0)) { // Node 0.
					// Do nothing here. PDU will be transmitted to node 1.
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(1)) { // Node 1.
					// End propagation.
					link01.endPropagation(pduFromEventEntityNonConst);
					// PDU will be transmitted to next node according to explicit route.
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // Node 2.
					// End propagation.
					// Final destination.
					link12.endPropagation(pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 3.
					// End propagation.
					// Final destination.
					link13.endPropagation(pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 4.
					// End propagation.
					// Final destination.
					link14.endPropagation(pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 5.
					// End propagation.
					// Final destination.
					link15.endPropagation(pduFromEventEntityNonConst);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 6.
					// End propagation.
					// Final destination.
					link16.endPropagation(pduFromEventEntityNonConst);
				}
				// This should be the same for either route. The "next" node should to the processing; it is actually the "current" node at this stage/event.
				if (pduFromEventEntityNonConst->next != nodeVector.at(2) && pduFromEventEntityNonConst->next != nodeVector.at(3) &&
					pduFromEventEntityNonConst->next != nodeVector.at(4) && pduFromEventEntityNonConst->next != nodeVector.at(5) &&
					pduFromEventEntityNonConst->next != nodeVector.at(6)) {
					// Not Final destination; forward it.
					EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
					scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				} else {
					// Final destination. Do not schedule anything else.
					EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(std::dynamic_pointer_cast<ProtocolDataUnit>(eventEntityNonConst)));
					// Test for recorded routes.
					if (pduFromEventEntityNonConst->next == nodeVector.at(2)) { // Node 2.
						// Final destination.
						EXPECT_EQ("constant",  std::dynamic_pointer_cast<Message>(pduFromEventEntityNonConst->associatedEntity)->getContents());
						EXPECT_EQ(routeToNode2, pduFromEventEntityNonConst->getRecordedRoute());
					} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 3.
						// Final destination.
						EXPECT_EQ("exponential",  std::dynamic_pointer_cast<Message>(pduFromEventEntityNonConst->associatedEntity)->getContents());
						EXPECT_EQ(routeToNode3, pduFromEventEntityNonConst->getRecordedRoute());
					} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 4.
						// Final destination.
						EXPECT_EQ("normal",  std::dynamic_pointer_cast<Message>(pduFromEventEntityNonConst->associatedEntity)->getContents());
						EXPECT_EQ(routeToNode4, pduFromEventEntityNonConst->getRecordedRoute());
					} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 5.
						// Final destination.
						EXPECT_EQ("qcn",  std::dynamic_pointer_cast<Message>(pduFromEventEntityNonConst->associatedEntity)->getContents());
						EXPECT_EQ(routeToNode5, pduFromEventEntityNonConst->getRecordedRoute());
					} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) { // Node 6.
						// Final destination.
						EXPECT_EQ("weibull",  std::dynamic_pointer_cast<Message>(pduFromEventEntityNonConst->associatedEntity)->getContents());
						EXPECT_EQ(routeToNode6, pduFromEventEntityNonConst->getRecordedRoute());
					}
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
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(3)) {
					EXPECT_EQ(link13.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(4)) {
					EXPECT_EQ(link14.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(5)) {
					EXPECT_EQ(link15.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else if (pduFromEventEntityNonConst->next == nodeVector.at(6)) {
					EXPECT_EQ(link16.propagatePdu(EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst), LinkReturnType::PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED);
				} else {
					EXPECT_FALSE(true); // Fail here.
				}
				break;

			default:
				EXPECT_FALSE(true); // This should never be reached.
				break;
		}
	} // End while.
}