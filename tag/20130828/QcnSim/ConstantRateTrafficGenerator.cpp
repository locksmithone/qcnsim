/**
 * @author Marcos Portnoi
 * @date August 2013
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

#include "ConstantRateTrafficGenerator.h"

/**
 * Constructor with parameter.
 *
 * @details
 * Since this is a constant-rate traffic generator, there is no need for random numbers, nor seed. Therefore, there is really no need to use
 * SimulatorGlobals object; however, we need to request it to pass to TrafficGenerator constructor, since it uses a reference to this object.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, necessary to fullfil TraffiGenerator constructor reference parameter.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param interval Fixed, constant time interval for this generator.
 */
ConstantRateTrafficGenerator::ConstantRateTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double interval): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority), interval(interval) {
}

/**
 * Creates an instance of constant rate traffic event.
 *
 * @details
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> ConstantRateTrafficGenerator::createInstanceTrafficEvent() {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent();
	if (token != NULL) {
		scheduler.schedule(Event(interval, eventType, token));
	}
	return token;
}

/**
 * Creates an instance of constant rate traffic event.
 *
 * @details
 * Calls parent function and complements with event generation and exponential random variate.
 * Uses parameter tokenContents.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> ConstantRateTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents);
	if (token != NULL) {
		scheduler.schedule(Event(interval, eventType, token));
	}
	return token;
}

/**
 * Creates an instance of constant rate traffic event.
 *
 * @details
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents and parameter explicit route.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> ConstantRateTrafficGenerator::createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute);
	if (token != NULL) {
		scheduler.schedule(Event(interval, eventType, token));
	}
	return token;
}

/**
 * Creates an instance of constant rate traffic event.
 *
 * @details
 * Calls parent function and complements with event generation and exponential random variate.
 * Uses parameter tokenContents and parameter explicit route.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> ConstantRateTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute);
	if (token != NULL) {
		scheduler.schedule(Event(interval, eventType, token));
	}
	return token;
}

/**
 * Returns interval (constant time interval between events).
 *
 * @return Interval.
 */
double ConstantRateTrafficGenerator::getInterval() const {
	return interval;
}
