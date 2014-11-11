/**
 * @author Marcos Portnoi
 * @date May 2013
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

// QcnSim.cpp : Defines the entry point for the console application.
//

#include "QcnSim.h"

int main() {
	// Build simulation objects.
	double serviceTimeNetworkLinks = 0.1; // Service time for network links.
	double serviceTimeCentralServer = 0.05; // Service time for central server.
	double interarrivalTime = 1.0; // Interarrival time for Poisson traffic generator.
	unsigned int seed = 1; // Seed for random number generator.
	double maxSimulationTime = 500.0; // Maximum simulation time.
	bool simulationEnded = false; // Indicates the simulation has ended.
	Event currentEvent;
	//std::map<std::shared_ptr<Facility>, std::shared_ptr<ExponentialTrafficGenerator>> sourceAndLink;
	//std::map<std::shared_ptr<Facility>, std::shared_ptr<ExponentialTrafficGenerator>>::iterator sourceAndLinkIterator; // Create iterator for map.
	std::map<std::shared_ptr<Facility>, ExponentialTrafficGenerator> sourceAndLink;
	std::map<std::shared_ptr<Facility>, ExponentialTrafficGenerator>::iterator sourceAndLinkIterator; // Create iterator for map.
	std::shared_ptr<Facility> facility;
	std::shared_ptr<Token> token;
	
	//ExponentialTrafficGenerator exponentialGenerator;
	unsigned int totalTokensArrivedAtCentralFacility = 0;
	unsigned int totalTokensArrivedAtBackupFacility = 0;
	double arrivalRate = 0;
	int numberOfGenerators = 100; // Number of generators and attached links.
	int counter = 0;
	bool isBackupServerActive = false;

	// A few random variates.
	std::exponential_distribution<double> exponentialVariateLink(1/serviceTimeNetworkLinks);  // The exponential generator takes lambda (Poisson), not tau!
	std::exponential_distribution<double> exponentialVariateCentralServer(1/serviceTimeCentralServer);  // The exponential generator takes lambda (Poisson), not tau!
	std::uniform_int_distribution<int> uniformVariate(0,1); // 50% probability generator.


	// SimulatorGlobals.
	SimulatorGlobals simulatorGlobals(0.0, 0.0, false, "Multiple links");

	// Scheduler
	Scheduler scheduler(simulatorGlobals);

	// Create one central facility with 4 servers, one similar backup facility.
	std::shared_ptr<Facility> centralFacility(new Facility("Central Facility", 4, simulatorGlobals, scheduler));
	std::shared_ptr<Facility> backupFacility(new Facility("Backup Facility", 4, simulatorGlobals, scheduler));

	// Set up token contents and some dummy entities.
	std::shared_ptr<Message> tokenContents(new Message("Message within token"));
	std::shared_ptr<Message> source(new Message("Source"));
	
	// Create facilities and attach exponential generators to simulate simplex network links and message generators.
	for (int i = 0; i < numberOfGenerators; ++i) {
		std::shared_ptr<Facility> facility(new Facility("Simplex link", simulatorGlobals, scheduler));
		// Notice that routing is still not implemented, that is why source and destination are the same for this code.
		ExponentialTrafficGenerator exponentialGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, facility, facility, 1, interarrivalTime, seed);
		//std::shared_ptr<ExponentialTrafficGenerator> exponentialGenerator(new ExponentialTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, facility, facility, 1, interarrivalTime, seed));
		//sourceAndLink.insert(std::pair<std::shared_ptr<Facility>,std::shared_ptr<ExponentialTrafficGenerator>>(facility, exponentialGenerator));
		sourceAndLink.insert(std::pair<std::shared_ptr<Facility>,ExponentialTrafficGenerator>(facility, exponentialGenerator));
	}

	
	// One exponential traffic generator
	//ExponentialTrafficGenerator exponentialGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, facility, 1, interarrivalTime, seed);
	
	// Build initial events.
	scheduler.schedule(Event(0.0, EventType::BEGIN_SIMULATION, NULL));
	scheduler.schedule(Event(maxSimulationTime, EventType::END_SIMULATION, NULL));
	scheduler.schedule(Event(maxSimulationTime/2, EventType::ACTIVATE_BACKUP_FACILITY, NULL));

	// Main while loop to control events.
	while (! simulationEnded) {
		currentEvent = scheduler.cause(); // Fetch next event from event chain/scheduler.
		// Decide on what to do based on EventType.
		switch (currentEvent.eventType) {

		case EventType::BEGIN_SIMULATION:
			// Turn on traffic generators, schedule an initial arrival.
			//for (auto keyValue : sourceAndLink) {
			for (auto &keyValue : sourceAndLink) {
				//keyValue.second->turnOn();
				//keyValue.second->createInstanceTrafficEvent();
				keyValue.second.turnOn();
				keyValue.second.createInstanceTrafficEvent();
			}
			break;

		case EventType::TRAFFIC_GENERATOR_ARRIVAL:
			// Arrival from traffic generator. Schedule request service for this token.
			scheduler.schedule(Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, std::dynamic_pointer_cast<const Token>(currentEvent.entity) ));
			// Schedule new arrival from generator.
			sourceAndLinkIterator=sourceAndLink.find(std::dynamic_pointer_cast<Facility>(std::dynamic_pointer_cast<const Token>(currentEvent.entity)->source)); // Find the attached generator to current facility.
			//sourceAndLinkIterator->second->createInstanceTrafficEvent();
			sourceAndLinkIterator->second.createInstanceTrafficEvent();
			break;

		case EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY:
			// Request service for current token. If token was successfully put into service, schedules release. Otherwise, do nothing (it was probably enqueued)>
			if (std::dynamic_pointer_cast<Facility>(std::dynamic_pointer_cast<const Token>(currentEvent.entity)->destination)->request(std::dynamic_pointer_cast<const Token>(currentEvent.entity), EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY) == FacilityReturnType::TOKEN_PUT_IN_SERVICE) {
				scheduler.schedule(Event(exponentialVariateLink(simulatorGlobals.getRandomNumberGeneratorEngineInstance()), EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, std::dynamic_pointer_cast<const Token>(currentEvent.entity)));
			}
			break;

		case EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY:
			// Release token from service at facility.
			//facility->release(std::dynamic_pointer_cast<const Token>(currentEvent.entity));
			std::dynamic_pointer_cast<Facility>(std::dynamic_pointer_cast<const Token>(currentEvent.entity)->destination)->release(std::dynamic_pointer_cast<const Token>(currentEvent.entity));
			
			// Change token destination if not yet at Central Facility or Backup Facility.
			token = std::dynamic_pointer_cast<Token>(std::const_pointer_cast<Entity>(currentEvent.entity));
			if (token->destination != centralFacility && token->destination != backupFacility) { // Route to central facility or bakcup, otherwise, done.
				if (isBackupServerActive) {
					if (uniformVariate(simulatorGlobals.getRandomNumberGeneratorEngineInstance()) == 1) {
						token->destination = centralFacility;
						// Schedule next event for the token.
						scheduler.schedule(Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_CENTRAL_FACILITY, token));
					} else {
						token->destination = backupFacility;
						// Schedule next event for the token.
						scheduler.schedule(Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_BACKUP_FACILITY, token));
					}
				} else {
					token->destination = centralFacility;
					// Schedule next event for the token.
					scheduler.schedule(Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_CENTRAL_FACILITY, token));
				}
			}
			break;

		case EventType::REQUEST_SERVICE_FOR_TOKEN_AT_CENTRAL_FACILITY:
			// Increment arrival count here.
			++totalTokensArrivedAtCentralFacility;
			// Request service for current token at central server. If token was successfully put into service, schedules release. Otherwise, do nothing (it was probably enqueued).
			if (std::dynamic_pointer_cast<Facility>(std::dynamic_pointer_cast<const Token>(currentEvent.entity)->destination)->request(std::dynamic_pointer_cast<const Token>(currentEvent.entity), EventType::REQUEST_SERVICE_FOR_TOKEN_AT_CENTRAL_FACILITY) == FacilityReturnType::TOKEN_PUT_IN_SERVICE) {
				scheduler.schedule(Event(exponentialVariateCentralServer(simulatorGlobals.getRandomNumberGeneratorEngineInstance()), EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, std::dynamic_pointer_cast<const Token>(currentEvent.entity)));
			}
			break;

		case EventType::REQUEST_SERVICE_FOR_TOKEN_AT_BACKUP_FACILITY:
			// Increment arrival count here.
			++totalTokensArrivedAtBackupFacility;
			// Request service for current token at central server. If token was successfully put into service, schedules release. Otherwise, do nothing (it was probably enqueued).
			if (std::dynamic_pointer_cast<Facility>(std::dynamic_pointer_cast<const Token>(currentEvent.entity)->destination)->request(std::dynamic_pointer_cast<const Token>(currentEvent.entity), EventType::REQUEST_SERVICE_FOR_TOKEN_AT_BACKUP_FACILITY) == FacilityReturnType::TOKEN_PUT_IN_SERVICE) {
				scheduler.schedule(Event(exponentialVariateCentralServer(simulatorGlobals.getRandomNumberGeneratorEngineInstance()), EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, std::dynamic_pointer_cast<const Token>(currentEvent.entity)));
			}
			break;

		case EventType::ACTIVATE_BACKUP_FACILITY:
			// Activate backup facility, will reroute half of the generators to the backup facility from now on.
			isBackupServerActive = true;
			break;

		case EventType::END_SIMULATION:
			// End simulation by setting flag.
			simulationEnded = true;
			break;
		}
	}

	// Print a few statistics.
	
	
	std::cout << "Multiple links\n\n" << std::endl;
	std::cout << "Simulated time: " << simulatorGlobals.getCurrentAbsoluteTime() << std::endl;
	std::cout << "\nExponential Source Stats:" << std::endl;
	std::cout << "-------------------------" << std::endl;
	for (auto &keyValue : sourceAndLink) {
		//std::cout << "Tokens generated: " << keyValue.second->getTokensGeneratedCount() << std::endl;
		//std::cout << "Mean interarrival time: " << keyValue.second->getTau() << std::endl;
		std::cout << "Tokens generated: " << keyValue.second.getTokensGeneratedCount() << std::endl;
		std::cout << "Mean interarrival time: " << keyValue.second.getTau() << std::endl;
	}
	std::cout << "\nServers:" << std::endl;
	std::cout << "--------" << std::endl;
	std::cout << "Mean service time network links: " << serviceTimeNetworkLinks << std::endl;
	std::cout << "Mean service time central facility: " << serviceTimeCentralServer << std::endl;
	for (auto &keyValue : sourceAndLink) {
		std::cout << "Utilization: " << keyValue.first->getUtilization() << std::endl;
		std::cout << "Tokens served: " << keyValue.first->getFullyServicedTokensCount() << std::endl;
		std::cout << "Mean service rate: " << keyValue.first->getMeanServiceRate() << std::endl;
		//arrivalRate = keyValue.second->getTokensGeneratedCount() / simulatorGlobals.getCurrentAbsoluteTime();
		arrivalRate = keyValue.second.getTokensGeneratedCount() / simulatorGlobals.getCurrentAbsoluteTime();
		std::cout << "Arrival rate: " << arrivalRate << std::endl;
		std::cout << "Mean queue size: " << keyValue.first->getMeanQueueLength() << std::endl;
		std::cout << "Mean queue wait time: " << keyValue.first->getMeanQueueLength() / arrivalRate << std::endl;
		std::cout << "Max queue size: " << keyValue.first->getMaxQueueSize() << std::endl;
	}
		
	
	std::cout << "\nCentral facility queue" << std::endl;
	std::cout << "----------------------" << std::endl;
	std::cout << "Tokens arrived: " << totalTokensArrivedAtCentralFacility << std::endl;
	arrivalRate = totalTokensArrivedAtCentralFacility / simulatorGlobals.getCurrentAbsoluteTime();
	std::cout << "Arrival rate: " << arrivalRate << std::endl;
	std::cout << "Mean queue size: " << centralFacility->getMeanQueueLength() << std::endl;
	std::cout << "Mean queue wait time: " << centralFacility->getMeanQueueLength() / arrivalRate << std::endl;
	std::cout << "Max queue size: " << centralFacility->getMaxQueueSize() << std::endl;

	std::cout << "\nBackup facility queue" << std::endl;
	std::cout << "----------------------" << std::endl;
	std::cout << "Tokens arrived: " << totalTokensArrivedAtBackupFacility << std::endl;
	arrivalRate = totalTokensArrivedAtBackupFacility / simulatorGlobals.getCurrentAbsoluteTime();
	std::cout << "Arrival rate: " << arrivalRate << std::endl;
	std::cout << "Mean queue size: " << backupFacility->getMeanQueueLength() << std::endl;
	std::cout << "Mean queue wait time: " << backupFacility->getMeanQueueLength() / arrivalRate << std::endl;
	std::cout << "Max queue size: " << backupFacility->getMaxQueueSize() << std::endl;
	
	

	return 0;
}

