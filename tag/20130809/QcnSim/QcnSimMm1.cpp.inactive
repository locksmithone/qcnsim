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
	double serviceTime = 0.1; // Service time for single server.
	double interarrivalTime = 0.4; // Interarrival time for Poisson traffic generator.
	unsigned int seed = 1; // Seed for random number generator.
	double maxSimulationTime = 500000.0; // Maximum simulation time.
	bool simulationEnded = false; // Indicates the simulation has ended.
	Event currentEvent;

	// A few random variates.
	std::exponential_distribution<double> exponentialVariate(1/serviceTime);  // The exponential generator takes lambda (Poisson), not tau!


	// SimulatorGlobals.
	SimulatorGlobals simulatorGlobals(0.0, 0.0, false, "M/M/1 Queue");

	// Scheduler
	Scheduler scheduler(simulatorGlobals);

	// One facility with one server.
	std::shared_ptr<Facility> facility(new Facility("Single server facility", simulatorGlobals, scheduler));

	// Set up token contents and some dummy entities.
	std::shared_ptr<Message> tokenContents(new Message("Message within token"));
	std::shared_ptr<Message> source(new Message("Source"));
	
	// One exponential traffic generator
	ExponentialTrafficGenerator exponentialGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, facility, 1, interarrivalTime, seed);
	
	// Build initial events.
	scheduler.schedule(Event(0.0, EventType::BEGIN_SIMULATION, NULL));
	scheduler.schedule(Event(maxSimulationTime, EventType::END_SIMULATION, NULL));

	// Main while loop to control events.
	while (! simulationEnded) {
		currentEvent = scheduler.cause(); // Fetch next event from event chain/scheduler.
		// Decide on what to do based on EventType.
		switch (currentEvent.eventType) {

		case EventType::BEGIN_SIMULATION:
			// Turn on traffic generator, schedule an initial arrival.
			exponentialGenerator.turnOn();
			exponentialGenerator.createInstanceTrafficEvent();
			break;

		case EventType::TRAFFIC_GENERATOR_ARRIVAL:
			// Arrival from traffic generator. Schedule request service for this token.
			scheduler.schedule(Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, std::dynamic_pointer_cast<const Token>(currentEvent.entity) ));
			// Schedule new arrival from generator.
			exponentialGenerator.createInstanceTrafficEvent();
			break;

		case EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY:
			// Request service for current token. If token was successfully put into service, schedules release. Otherwise, do nothing.
			if (std::dynamic_pointer_cast<Facility>(std::dynamic_pointer_cast<const Token>(currentEvent.entity)->destination)->request(std::dynamic_pointer_cast<const Token>(currentEvent.entity), EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY) == FacilityReturnType::TOKEN_PUT_IN_SERVICE) {
				scheduler.schedule(Event(exponentialVariate(simulatorGlobals.getRandomNumberGeneratorEngineInstance()), EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, std::dynamic_pointer_cast<const Token>(currentEvent.entity)));
			}
			break;

		case EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY:
			// Release token from service at facility.
			facility->release(std::dynamic_pointer_cast<const Token>(currentEvent.entity));
			break;

		case EventType::END_SIMULATION:
			// End simulation by setting flag.
			simulationEnded = true;
			break;
		}
	}

	// Print a few statistics.
	std::cout << "M/M/1 Queue\n\n" << std::endl;
	std::cout << "Simulated time: " << simulatorGlobals.getCurrentAbsoluteTime() << std::endl;
	std::cout << "\nExponential Source Stats:" << std::endl;
	std::cout << "-------------------------" << std::endl;
	std::cout << "Tokens generated: " << exponentialGenerator.getTokensGeneratedCount() << std::endl;
	std::cout << "Mean interarrival time: " << exponentialGenerator.getTau() << std::endl;
	std::cout << "\nServers:" << std::endl;
	std::cout << "--------" << std::endl;
	std::cout << "Mean service time: " << serviceTime << std::endl;
	std::cout << "Utilization: " << facility->getUtilization() << std::endl;
	std::cout << "Tokens served: " << facility->getFullyServicedTokensCount() << std::endl;
	std::cout << "Mean service rate: " << facility->getMeanServiceRate() << std::endl;
	std::cout << "\nQueues" << std::endl;
	std::cout << "------" << std::endl;
	double arrivalRate = exponentialGenerator.getTokensGeneratedCount() / simulatorGlobals.getCurrentAbsoluteTime();
	std::cout << "Arrival rate: " << arrivalRate << std::endl;
	std::cout << "Mean queue size: " << facility->getMeanQueueLength() << std::endl;
	std::cout << "Mean queue wait time: " << facility->getMeanQueueLength() / arrivalRate << std::endl;
	std::cout << "Max queue size: " << facility->getMaxQueueSize() << std::endl;
	

	return 0;
}

