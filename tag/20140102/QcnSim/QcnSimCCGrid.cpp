/**
 * @author Marcos Portnoi
 * @date November 2013
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

/**
 * @brief Simulation for CCGrid 2014 paper.
 *
 * @details 
 * Scenario B, simulation 1, no network failures. One BOINC server/destination per region ID.
 */

#include "QcnSimCCGrid.h"

// A few constants...
#define INPUT_FILENAME "..\\Release\\map C high (4 regions) no failure 1.csv"
#define OUTPUT_FILENAME std::string(INPUT_FILENAME) + "-output.csv"
#define PDU_SIZE 512 // PDU size for PDUs.
#define BANDWIDTH_REGION_A 8700000 // 7 Mbps.
#define PROPAGATION_REGION_A 0.03 // 30 ms.
#define BANDWIDTH_REGION_B 3000000 // 5 Mbps.
#define PROPAGATION_REGION_B 0.05
#define BANDWIDTH_REGION_C 5000000 // 5 Mbps.
#define PROPAGATION_REGION_C 0.05
#define BANDWIDTH_REGION_D 5000000 // 5 Mbps.
#define PROPAGATION_REGION_D 0.05
#define BANDWIDTH_REGION_FAKE 6000000 // 6 Mbps.
#define PROPAGATION_REGION_FAKE 0.07 // 70 ms.
#define REGION_A_ID 1 // Region ID as defined by qcnexplorer. This is dependent of the qcnexplorer build, check the input file.
#define REGION_B_ID 2 // Region ID as defined by qcnexplorer.
#define REGION_C_ID 5 // Region ID as defined by qcnexplorer.
#define REGION_D_ID 4 // Region ID as defined by qcnexplorer.
#define REGION_FAKE_ID 888 // Region ID for a fake region, used to reroute PDUs in mid-simulation.
#define REGION_A_SOURCE REGION_A_ID // Region source node key. Same as region ID.
#define REGION_B_SOURCE REGION_B_ID // Region source node key. Same as region ID.
#define REGION_C_SOURCE REGION_C_ID // Region source node key. Same as region ID.
#define REGION_D_SOURCE REGION_D_ID // Region source node key.
#define REGION_FAKE_SOURCE REGION_FAKE_ID
#define REGION_A_DESTINATION (REGION_A_ID * 10) // Region destination node key. Multiply the source * 10 to get a different key as the source.
#define REGION_B_DESTINATION (REGION_B_ID * 10) // Region destination node key.
#define REGION_C_DESTINATION (REGION_C_ID * 10) // Region destination node key.
#define REGION_D_DESTINATION (REGION_D_ID * 10) // Region destination node key.
#define REGION_FAKE_DESTINATION (REGION_FAKE_ID * 10)
//#define REGION_D_DESTINATION REGION_C_DESTINATION // Region destination node key.
#define MAX_SIMULATION_TIME 50.0 // Maximum simulation time in seconds.
#define LINK_DOWN_TIME 1.0 // Network failure time of occurrence after first QCN traffic arrival (thus, relative time).
#define LINK_FAILURE false // If true, link will be set down at time LINK_DOWN_TIME after first QCN traffic arrival.
#define REROUTE_TRAFFIC_TIME 1.0 // Route will be rerouted after this time after first traffic arrival.
#define REROUTE_TRAFFIC false // If true, then traffic will be rerouted accordingn to LINK_DOWN
#define PRINT_TRACE false

/**
 * @brief Finds and returns a link pointer form a map of links, given the nodeB.
 *
 * @param linkMap Reference to map of links.
 * @param nodeB Pointer to nodeB from a link.
 *
 * @return Pointer to found link, or nullptr if not found.
 */
std::shared_ptr<Link> findLinkByNodeB(std::map<unsigned int, std::shared_ptr<Link>> linkMap, std::shared_ptr<Node> nodeB) {
	for (auto linkMapIterator : linkMap) {
		if (linkMapIterator.second->getNodeB() == nodeB) {
			return linkMapIterator.second;
		}
	}
	return nullptr; // Not found. This will cause an error later in the code.
}

/**
 * @brief Main simulation function.
 */
int main(int argc, char *argv[]) {
	std::ofstream outputFile; // Output file handle
	std::ifstream inputFile; // Input file handle.
	std::string line; // Line to be read from input file.
	std::map<unsigned int, std::shared_ptr<QcnSensorTrafficGenerator>> qcnSensorTrafficGeneratorMap; // Map to hold all qcnSensorTrafficGenerators.
	std::shared_ptr<SeismicEventData> seismicEventData(nullptr); // Will hold seismic event objects instantiated from file.
	std::map<unsigned int, std::shared_ptr<Node>> nodeMap;
	std::map<unsigned int, std::shared_ptr<Link>> linkMap;
	std::map<unsigned int, std::vector<std::shared_ptr<Entity>>> explicitRouteMap;
	std::shared_ptr<Link> link(nullptr);
	std::uniform_int_distribution<int> uniformVariate(0,1); // 50% probability generator.
	
	bool simulationEnded = false; // Indicates whether the simulation has ended.
	Event currentEvent;
	bool rerouteTrafficEventFulfilled = false; // Indicates whether this event was already fulfilled.
	bool setLinkDownEventFulfilled = false; // Indicates whether this event was already fulfilled.
	//std::shared_ptr<ProtocolDataUnit> pduFromEventEntity(nullptr);
	std::shared_ptr<ProtocolDataUnit> pduFromEventEntityNonConst(nullptr); // Will handle the pdu extracted from Event objects, removed const-ness.
	
	// SimulatorGlobals, instantiate here.
	SimulatorGlobals simulatorGlobals(0.0, 0.0, false, "CCGrid 2014 - Map C, no failure.");
	// Scheduler.
	Scheduler scheduler(simulatorGlobals);

	// Create 4 nodes, one for each region ID. Insert into node Map. Key is Region ID or Region Source Node.
	if (PRINT_TRACE) {
		std::cout << "Creating nodes..." << std::endl;
	}
	// 3 Meta nodes, regions C and D share one. 3 Boinc servers/destination nodes.
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_A_SOURCE, std::make_shared<Node>(Node(simulatorGlobals))));
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_B_SOURCE, std::make_shared<Node>(Node(simulatorGlobals))));
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_C_SOURCE, std::make_shared<Node>(Node(simulatorGlobals))));
	////nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_D_SOURCE, std::make_shared<Node>(Node(simulatorGlobals))));
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_D_SOURCE, nodeMap.at(REGION_C_SOURCE))); // Note that this entry maps to same Node as Region C.
	//// Create 4 nodes to represent the destination BOINC servers; key will be regionID * 10. Note that if a destination already exists
	//// (case of two regions sharing one destination), the element is simply not inserted.
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_A_DESTINATION, std::make_shared<Node>(Node(simulatorGlobals))));
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_B_DESTINATION, std::make_shared<Node>(Node(simulatorGlobals))));
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_C_DESTINATION, std::make_shared<Node>(Node(simulatorGlobals))));
	////nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_D_DESTINATION, std::make_shared<Node>(Node(simulatorGlobals))));
	//nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_D_DESTINATION, nodeMap.at(REGION_C_DESTINATION))); // Note that his entry maps to the same node as Region C.
	
	// One meta-node, one BOINC server.
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_A_SOURCE, std::make_shared<Node>(Node(simulatorGlobals))));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_B_SOURCE, nodeMap.at(REGION_A_SOURCE)));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_C_SOURCE, nodeMap.at(REGION_A_SOURCE)));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_D_SOURCE, nodeMap.at(REGION_A_SOURCE))); 
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_FAKE_SOURCE, std::make_shared<Node>(Node(simulatorGlobals)))); 
	// Create 4 nodes to represent the destination BOINC servers; key will be regionID * 10. Note that if a destination already exists
	// (case of two regions sharing one destination), the element is simply not inserted.
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_A_DESTINATION, std::make_shared<Node>(Node(simulatorGlobals))));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_B_DESTINATION, nodeMap.at(REGION_A_DESTINATION)));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_C_DESTINATION, nodeMap.at(REGION_A_DESTINATION)));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_D_DESTINATION, nodeMap.at(REGION_A_DESTINATION)));
	nodeMap.insert(std::pair<unsigned int, std::shared_ptr<Node>>(REGION_FAKE_DESTINATION, std::make_shared<Node>(Node(simulatorGlobals))));
	
	if (PRINT_TRACE) {
		std::cout << nodeMap.size() << " nodes created." << std::endl;
	}

	// Create explicitRoutes. Key is Region ID. If regions share same source or destination, explicit routes might be the same, which is ok.
	if (PRINT_TRACE) {
		std::cout << "Creating explicit routes..." << std::endl;
	}
	// 3 explicit routes connecting meta-nodes to destinations.
	//explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_A_ID, std::vector<std::shared_ptr<Entity>>({ nodeMap.at(REGION_A_SOURCE), nodeMap.at(REGION_A_DESTINATION) })));
	//explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_B_ID, std::vector<std::shared_ptr<Entity>>({ nodeMap.at(REGION_B_SOURCE), nodeMap.at(REGION_B_DESTINATION) })));
	//explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_C_ID, std::vector<std::shared_ptr<Entity>>({ nodeMap.at(REGION_C_SOURCE), nodeMap.at(REGION_C_DESTINATION) })));
	////explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_D_ID, std::vector<std::shared_ptr<Entity>>({ nodeMap.at(REGION_D_SOURCE), nodeMap.at(REGION_D_DESTINATION) })));
	//explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_D_ID, explicitRouteMap.at(REGION_C_ID))); // Same route as Region C.
	
	// One explicit route between meta-node and destination.
	explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_A_ID, std::vector<std::shared_ptr<Entity>>({ nodeMap.at(REGION_A_SOURCE), nodeMap.at(REGION_A_DESTINATION) })));
	explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_B_ID, explicitRouteMap.at(REGION_A_ID)));
	explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_C_ID, explicitRouteMap.at(REGION_A_ID)));
	explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_D_ID, explicitRouteMap.at(REGION_A_ID))); // Same route as Region C.
	explicitRouteMap.insert(std::pair<unsigned int, std::vector<std::shared_ptr<Entity>>>(REGION_FAKE_ID, std::vector<std::shared_ptr<Entity>>({ nodeMap.at(REGION_FAKE_SOURCE), nodeMap.at(REGION_FAKE_DESTINATION) })));
	
	if (PRINT_TRACE) {
		std::cout << explicitRouteMap.size() << " explicit routes created." << std::endl;
	}

	// Create simplex links to connect each region node to its destination. Put into link map. The key will be the destination node key.
	// If key already exists (link already created?), then insertion will simply not do anything.
	if (PRINT_TRACE) {
		std::cout << "Creating links..." << std::endl;
	}
	// 3 Links connecting meta-nodes to destinations.
	//linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_A_DESTINATION, std::make_shared<Link>(Link(nodeMap.at(REGION_A_SOURCE), nodeMap.at(REGION_A_DESTINATION),
	//	BANDWIDTH_REGION_A, PROPAGATION_REGION_A, simulatorGlobals, scheduler, "link region A"))));
	//linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_B_DESTINATION, std::make_shared<Link>(Link(nodeMap.at(REGION_B_SOURCE), nodeMap.at(REGION_B_DESTINATION),
	//	BANDWIDTH_REGION_B, PROPAGATION_REGION_B, simulatorGlobals, scheduler, "link region B"))));
	//linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_C_DESTINATION, std::make_shared<Link>(Link(nodeMap.at(REGION_C_SOURCE), nodeMap.at(REGION_C_DESTINATION),
	//	BANDWIDTH_REGION_C, PROPAGATION_REGION_C, simulatorGlobals, scheduler, "link region C and D"))));
	////linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_D_DESTINATION, std::make_shared<Link>(Link(nodeMap.at(REGION_D_SOURCE), nodeMap.at(REGION_D_DESTINATION),
	////	BANDWIDTH_REGION_D, PROPAGATION_REGION_D, simulatorGlobals, scheduler, "link region D"))));
	//linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_D_DESTINATION, linkMap.at(REGION_C_DESTINATION))); // Same link as Region C.
	
	// One link connecting one meta-node to destination.
	linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_A_DESTINATION, std::make_shared<Link>(Link(nodeMap.at(REGION_A_SOURCE), nodeMap.at(REGION_A_DESTINATION),
		BANDWIDTH_REGION_A, PROPAGATION_REGION_A, simulatorGlobals, scheduler, "link region A-B-C-D"))));
	linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_B_DESTINATION, linkMap.at(REGION_A_DESTINATION))); 
	linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_C_DESTINATION, linkMap.at(REGION_A_DESTINATION))); 
	linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_D_DESTINATION, linkMap.at(REGION_A_DESTINATION)));
	linkMap.insert(std::pair<unsigned int, std::shared_ptr<Link>>(REGION_FAKE_DESTINATION, std::make_shared<Link>(Link(nodeMap.at(REGION_FAKE_SOURCE), nodeMap.at(REGION_FAKE_DESTINATION),
		BANDWIDTH_REGION_FAKE, PROPAGATION_REGION_FAKE, simulatorGlobals, scheduler, "link fake-rerouting"))));
	
	if (PRINT_TRACE) {
		std::cout << linkMap.size() << " links created." << std::endl;
	}
			
	// Create seismic events, put them into event chain, and create QCN sensor traffic generators from the unique qcnExplorerSensorIds.
	// Open file for input.
	//inputFile.open(INPUT_FILENAME, std::ios::in);
	inputFile.open(argv[1], std::ios::in);  // Using command line argument.

	// Now read all file. Discard line if it begins with string "ID", meaning this is the header
	if (PRINT_TRACE) {
		std::cout << "Reading file to create seismic events and QCN sensor traffic generators..." << std::endl;
	}
	while (std::getline(inputFile, line)) {
		if (line.substr(0, 2) != "ID") {
			// It is not the header. Create seismic event object.
			seismicEventData = std::make_shared<SeismicEventData>(SeismicEventData(line, ','));
			// If QCN sensor with same sensor id does not yet exist, instantiate one and put into map.
			if (qcnSensorTrafficGeneratorMap.find(seismicEventData->qcnExplorerSensorId) == qcnSensorTrafficGeneratorMap.end()) {
				qcnSensorTrafficGeneratorMap.insert(std::pair<unsigned int, std::shared_ptr<QcnSensorTrafficGenerator>>(seismicEventData->qcnExplorerSensorId,
					std::make_shared<QcnSensorTrafficGenerator>(QcnSensorTrafficGenerator(simulatorGlobals, scheduler,
					EventType::TRAFFIC_GENERATOR_ARRIVAL, seismicEventData, nodeMap.at(seismicEventData->regionId), nodeMap.at(seismicEventData->regionId * 10),
					1, seismicEventData->latitude, seismicEventData->longitude, seismicEventData->qcnExplorerSensorId, seismicEventData->regionId))));
				qcnSensorTrafficGeneratorMap.at(seismicEventData->qcnExplorerSensorId)->turnOn(); // Have the generators ON by default.
			}
			scheduler.schedule(Event(seismicEventData->eventTime, EventType::SEISMIC_EVENT_DETECTION, seismicEventData));
		}
	}
	if (PRINT_TRACE) {
		std::cout << scheduler.getChainSize() << " seismic events entered into event chain.\n" << qcnSensorTrafficGeneratorMap.size() <<
			" QCN sensor traffic generators created." << std::endl;
	}
	inputFile.close();

	// Prepare output file for collecting general statistics.
	outputFile.open(std::string(argv[1]) + "-output.csv");
	outputFile << "link,ID,lat,lng,mag,obsvTime,hypoCentDist,regionID,deliverTime" << std::endl; // Write the header.

	// Schedule end of simulation.
	scheduler.schedule(Event(MAX_SIMULATION_TIME, EventType::END_SIMULATION, nullptr));
	
	// Main while-switch-case structure.
	if (PRINT_TRACE) {
		std::cout << "Beginning simulation..." << std::endl;
	}
	while (!simulationEnded) {
		currentEvent = scheduler.cause(); // Fetch next event from scheduler.
		//pduFromEventEntity = std::dynamic_pointer_cast<const ProtocolDataUnit>(currentEvent.entity); // Entity from event converted to PDU.
		pduFromEventEntityNonConst = std::dynamic_pointer_cast<ProtocolDataUnit>(std::const_pointer_cast<Entity>(currentEvent.entity)); // Same PDU as above, but non-const.
		
		// Now decide what to do.
		switch (currentEvent.eventType) {
			case EventType::REROUTE_QCN_TRAFFIC:
				// This event is only scheduled if macro REROUTE_TRAFFIC is true.
				// Flag is set, and then traffic arrivals will be directly modified.
				// Test here if traffic is to be forcibly rerouted. Use 50% probability. If true, modify seismic data region ID and source/destination nodes.
				for (auto qcnSensorTrafficGeneratorIterator : qcnSensorTrafficGeneratorMap) {
					if (uniformVariate(simulatorGlobals.getRandomNumberGeneratorEngineInstance()) == 1) {
						// Change qcn sensor generator with 50% probability.
						qcnSensorTrafficGeneratorIterator.second->setSource(nodeMap.at(REGION_FAKE_SOURCE));
						qcnSensorTrafficGeneratorIterator.second->setDestination(nodeMap.at(REGION_FAKE_DESTINATION));
						qcnSensorTrafficGeneratorIterator.second->setRegionId(REGION_FAKE_ID);
					}
				}
				break;

			case EventType::END_SIMULATION:
				if (PRINT_TRACE) {
					std::cout << "EventType::END_SIMULATION" << std::endl;
				}
				simulationEnded = true;
				break;

			case EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK:
				if (PRINT_TRACE) {
					std::cout << "EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK" << std::endl;
				}
				// End link transmission here and schedule end of propagation.
				findLinkByNodeB(linkMap, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next))->propagatePdu(EventType::END_PROPAGATION_AT_LINK, pduFromEventEntityNonConst);
				break;

			case EventType::PDUTOKEN_ARRIVAL_AT_NODE:
				if (PRINT_TRACE) {
					std::cout << "EventType::PDUTOKEN_ARRIVAL_AT_NODE" << std::endl;
				}
				// Ends propagation. If this node is just the source node, return will be nullptr, do nothing.
				link = findLinkByNodeB(linkMap, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next));
				if (link != nullptr) {
					link->endPropagation(pduFromEventEntityNonConst);
				}
				//findLinkByNodeB(linkMap, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next))->endPropagation(pduFromEventEntityNonConst);
				
				// Now forward the PDU to the next hop. Test, however, if this is the destination node, in which case output data to a file.
				if (std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next)->processAndForward(pduFromEventEntityNonConst) != NodeReturnType::FINAL_DESTINATION) {
					// Not final destination. Schedule transmission event.
					if (PRINT_TRACE) {
						std::cout << "EventType::PDUTOKEN_ARRIVAL_AT_NODE: not final destination." << std::endl;
					}
					scheduler.schedule(Event(0.0, EventType::REQUEST_PDU_TRANSMISSION_AT_LINK, pduFromEventEntityNonConst));
				} else {
					// It is the final destination. Output received data to file.
					// Or deliver it to some Facility modeling a BOINC server.
					if (PRINT_TRACE) {
						std::cout << "EventType::PDUTOKEN_ARRIVAL_AT_NODE: final destination." << std::endl;
					}
					seismicEventData = std::dynamic_pointer_cast<SeismicEventData>(pduFromEventEntityNonConst->associatedEntity);
					// Comment out the next line after debugging.
					outputFile << findLinkByNodeB(linkMap, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next))->getName() << ","; // For debug purposes only.
					outputFile << seismicEventData->qcnExplorerSensorId << ",";
					outputFile << std::setprecision(10) << seismicEventData->latitude << ",";
					outputFile << std::setprecision(10) << seismicEventData->longitude << ",";
					outputFile << std::setprecision(10) << seismicEventData->magnitude << ",";
					outputFile << std::setprecision(10) << seismicEventData->eventTime << ",";
					outputFile << std::setprecision(10) << seismicEventData->distance << ",";
					outputFile << seismicEventData->regionId << ",";
					outputFile << std::setprecision(10) << simulatorGlobals.getCurrentAbsoluteTime() << std::endl; // Timestamp of delivery at destination.
				}
				break;

			case EventType::REQUEST_PDU_TRANSMISSION_AT_LINK:
				if (PRINT_TRACE) {
					std::cout << "EventType::REQUEST_PDU_TRANSMISSION_AT_LINK" << std::endl;
				}
				// Decide which link to use based on next field of PDU.
				findLinkByNodeB(linkMap, std::dynamic_pointer_cast<Node>(pduFromEventEntityNonConst->next))->transmitPdu(EventType::REQUEST_PDU_TRANSMISSION_AT_LINK,
																												 EventType::END_TRANSMISSION_PROPAGATE_PDU_AT_LINK, pduFromEventEntityNonConst);
				break;

			case EventType::SEISMIC_EVENT_DETECTION:
				if (PRINT_TRACE) {
					std::cout << "EventType::SEISMIC_EVENT_DETECTION" << std::endl;
				}
				// When creating traffic instance, attach tokenContents (seismicEventData) and explicitRoute.
				// QCN sensor ID will the the key for the map. Upon seismic event, trigger message to send to BOINC server at destination.
				seismicEventData = std::dynamic_pointer_cast<SeismicEventData>(std::const_pointer_cast<Entity>(currentEvent.entity)); // Get seismic event data.
				qcnSensorTrafficGeneratorMap.at(seismicEventData->qcnExplorerSensorId)->createInstanceTrafficEventPdu(PDU_SIZE, seismicEventData,
					  explicitRouteMap.at(qcnSensorTrafficGeneratorMap.at(seismicEventData->qcnExplorerSensorId)->getRegionId()));				
				break;

			case EventType::SET_LINK_DOWN:
				if (PRINT_TRACE) {
					std::cout << "EventType::SET_LINK_DOWN" << std::endl;
				}
				// Set some link down here at specific time.
				linkMap.at(REGION_A_DESTINATION)->setDown();
				break;

			case EventType::TRAFFIC_GENERATOR_ARRIVAL:
				if (PRINT_TRACE) {
					std::cout << "EventType::TRAFFIC_GENERATOR_ARRIVAL" << std::endl;
				}
				// Arrival from traffic generator.
				// Actually, all operations here can be handled by PDUTOKEN_ARRIVAL_AT_NODE. Just schedule this as next event and pass PDU.
				// If desired, insert additional random propagation latency here, by scheduling PDUTOKEN_ARRIVAL_AT_NODE with the additional delay.
				// Do this only after generating QCN traffic, and the respective PDU arriving at the region meta-node.
				// For now, not adding any extra latency (0.0).
				scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst));
				// Schedule reroute traffic event is macro is set. Only once!
				if (REROUTE_TRAFFIC && !rerouteTrafficEventFulfilled) {
					scheduler.schedule(Event(REROUTE_TRAFFIC_TIME, EventType::REROUTE_QCN_TRAFFIC, nullptr));
					rerouteTrafficEventFulfilled = true; // Avoid doing this scheduling again.
				}
				// Schedule link down event if macro is set. Only once!
				if (LINK_FAILURE && !setLinkDownEventFulfilled) {
					scheduler.schedule(Event(LINK_DOWN_TIME, EventType::SET_LINK_DOWN, nullptr));
					setLinkDownEventFulfilled = true; // Avoid doing this scheduling again.
				}
				break;

			case EventType::END_PROPAGATION_AT_LINK:
				if (PRINT_TRACE) {
					std::cout << "EventType::END_PROPAGATION_AT_LINK" << std::endl;
				}
				// Ends propagation. Just deliver the PDU to the node by scheduling the next event.
				scheduler.schedule(Event(0.0, EventType::PDUTOKEN_ARRIVAL_AT_NODE, pduFromEventEntityNonConst));
				break;

			default:
				// This should not be reached.
				std::cout << "Error in Main switch-case!" << std::endl;
				return 1;
		} // End switch-case.
	} // End while.

	// Close output file for BOINC servers.
	outputFile.close();

	// Now print or record additional statistics here if desired.
	outputFile.open(std::string(argv[1]) + "-statistics.csv");

	outputFile << "Statistics" << std::endl;
	outputFile << "----------" << std::endl << std::endl;
	
	// Nodes.
	for (auto nodeMapIterator : nodeMap) {
		outputFile << "Node " << nodeMapIterator.first << std::endl;
		outputFile << "----------" << std::endl;
		outputFile << "Received bytes:      " << nodeMapIterator.second->getReceivedBytesCount() << std::endl;
		outputFile << "Received PDUs:       " << nodeMapIterator.second->getReceivedPdusOrTokensCount() << std::endl;
		outputFile << "Forwarded PDUs:      " << nodeMapIterator.second->getForwardedPdusOrTokensCount() << std::endl;
		outputFile << "Forwarded bytes:     " << nodeMapIterator.second->getForwardedBytesCount() << std::endl;
		outputFile << "Dropped PDUs:        " << nodeMapIterator.second->getDroppedPdusOrTokensCount() << std::endl;
		outputFile << "One-way delay:       " << std::setprecision(10) << nodeMapIterator.second->getLastPduOrTokenDelay() << std::endl;
		outputFile << "Mean one-way delay:  " << std::setprecision(10) << nodeMapIterator.second->getMeanPduOrTokenDelay() << std::endl;
		outputFile << "One-way jitter:      " << std::setprecision(10) << nodeMapIterator.second->getLastPduOrTokenJitter() << std::endl;
		outputFile << "Mean one-way jitter: " << std::setprecision(10) << nodeMapIterator.second->getMeanPduOrTokenJitter() << std::endl;
		outputFile << std::endl << std::endl;
	}

	// Links.
	for (auto linkMapIterator : linkMap) {
		outputFile << "Link " << linkMapIterator.first << " " << linkMapIterator.second->getName() << std::endl;
		outputFile << "-----------------------" << std::endl;
		outputFile << "Bandwidth (bps):           " << linkMapIterator.second->getBandwidth() << std::endl;
		outputFile << "Propagation delay (s):     " << linkMapIterator.second->getPropagationDelay() << std::endl;
		outputFile << "Dropped PDUs medium:       " << linkMapIterator.second->getDroppedPdusCountMedium() << std::endl;
		outputFile << "Dropped PDUs transmission: " << linkMapIterator.second->getDroppedPdusCountTransmissionServer() << std::endl;
		outputFile << "Dropped PDUs total:        " << linkMapIterator.second->getDroppedPdusCountWholeLink() << std::endl;
		outputFile << "Max recorded trans. queue: " << linkMapIterator.second->getMaxRecordedTransmissionQueueSize() << std::endl;
		outputFile << std::endl << std::endl;
	}



	return 0;
}

