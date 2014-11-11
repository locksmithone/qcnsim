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

#include "WeibullTrafficGenerator.h"

/**
 * Constructor with parameter.
 *
 * @details 
 * The seed for the random number generator is controlled by SimulatorGlobals. If none is ever specified, random_device output is used as seed.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, to get random generator info.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param scale Scale parameter of the Weibull distribution.
 * @param shape Shape parameter of the Weibull distribution.
 */
WeibullTrafficGenerator::WeibullTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double scale, double shape): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority), scale(scale), shape(shape) {
}

/**
 * Constructor with parameters, seed as parameter.
 *
 * @param simulatorGlobals Reference to SimulatorGlobals object, to get random generator info.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param scale Scale parameter of the Weibull distribution.
 * @param shape Shape parameter of the Weibull distribution.
 * @param seed Seed to the default random number generator, to be passed to SimulatorGlobals object.
 */
WeibullTrafficGenerator::WeibullTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double scale, double shape, unsigned int seed): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority, seed), scale(scale), shape(shape) {
	//this->tau = tau;
	//exponentialDistribution.;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details 
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses member variable tokenContents.
 *
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details 
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses parameter tokenContents.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details 
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses member variable tokenContents and parameter explicit route.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute, recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details 
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses parameter tokenContents and explicit route.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute, recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Generate a Weibull distribution variate based on member variables scale and shape.
 *
 * @details 
 * The Weibull variate is generated using C++11 Random library and uses the random number generator obtained from SimulatorGlobals,
 * such that its global seed can be utilized.
 *
 * @return Weibull distribution variate based on member variables scale and shape.
 */
double WeibullTrafficGenerator::generateWeibullVariate() {
	// Have not seen different behavior with or without static below, per unit testing.
	// The pseudorandom sequence seems to be unique per simulation, not per generator; the former is the expected behavior.
	std::weibull_distribution<double> weibullGenerator(shape, scale);
	return weibullGenerator(simulatorGlobals.getRandomNumberGeneratorEngineInstance());
}

/**
 * Returns scale (scale parameter).
 *
 * @return Scale.
 */
double WeibullTrafficGenerator::getScale() const {
	return scale;
}

/**
 * Returns shape (shape parameter).
 *
 * @return Shape.
 */
double WeibullTrafficGenerator::getShape() const {
	return shape;
}

/**
 * @brief Creates an instance of Weibull traffic event with PDU.
 *
 * @param pduSize Size of PDU to generate.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> WeibullTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, bool recordRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	// Notice that the base class member function "createInstance..." is used here instead of the derived override:
	// the derived override schedules an event, and that is undesired here, since an event can only be scheduled "after" the PDU is created.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(recordRoute);
	if (token == nullptr) {
		return nullptr;
	}
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(token, pduSize));
	//std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(recordRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of Weibull traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param pduContents Reference to Entity object that will be carried by the token.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> WeibullTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> pduContents,
																						   bool recordRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(pduContents, recordRoute);
	if (token == nullptr) {
		return nullptr;
	}
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(token, pduSize));
	//std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(pduContents, recordRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of Weibull traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> WeibullTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute,
																						   bool recordRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute, recordRoute);
	if (token == nullptr) {
		return nullptr;
	}
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(token, pduSize));
	//std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(explicitRoute, recordRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of Weibull traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> WeibullTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents,
																						   std::vector<std::shared_ptr<Entity>> explicitRoute,
																						   bool recordRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute, recordRoute);
	if (token == nullptr) {
		return nullptr;
	}
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(token, pduSize));
	//std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute, recordRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, pdu));
	}
	return pdu;
}