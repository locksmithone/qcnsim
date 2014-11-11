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

#include "NormalTrafficGenerator.h"

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
 * @param mean Mean of the normal distribution, or mu.
 * @param standardDeviation Standard deviation of the normal distribution, or sigma.
 */
NormalTrafficGenerator::NormalTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double mean, double standardDeviation): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority), mean(mean), standardDeviation(standardDeviation) {
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
 * @param mean Mean of the normal distribution, or mu.
 * @param standardDeviation Standard deviation of the normal distribution, or sigma.
 * @param seed Seed to the default random number generator, to be passed to SimulatorGlobals object.
 */
NormalTrafficGenerator::NormalTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double mean, double standardDeviation, unsigned int seed): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority, seed), mean(mean), standardDeviation(standardDeviation) {
}

/**
 * Creates an instance of Normal traffic event.
 *
 * @details 
 * Calls parent function and complements with event generation and Normal random variate.
 * Uses member variable tokenContents.
 *
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> NormalTrafficGenerator::createInstanceTrafficEvent(bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateNormalVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Normal traffic event.
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
std::shared_ptr<Token> NormalTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateNormalVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Normal traffic event.
 *
 * @details 
 * Calls parent function and complements with event generation and Normal random variate.
 * Uses member variable tokenContents and parameter explicit route.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> NormalTrafficGenerator::createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute, recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateNormalVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Normal traffic event.
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
std::shared_ptr<Token> NormalTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents,
																		  std::vector<std::shared_ptr<Entity>> explicitRoute,
																		  bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute, recordRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(generateNormalVariate(), eventType, token));
	}
	return token;
}

/**
 * Generate a Normal distribution variate based on member variables mean and standardDeviation.
 *
 * @details 
 * The Normal variate is generated using C++11 Random library and uses the random number generator obtained from SimulatorGlobals,
 * such that its global seed can be utilized.
 *
 * @return Normal distribution variate based on member variables mean and standardDeviation.
 */
double NormalTrafficGenerator::generateNormalVariate() {
	std::normal_distribution<double> normalGenerator(mean, standardDeviation);
	return normalGenerator(simulatorGlobals.getRandomNumberGeneratorEngineInstance());
}

/**
 * Returns mean or mu parameter.
 *
 * @return Mean.
 */
double NormalTrafficGenerator::getMean() const {
	return mean;
}

/**
 * Returns standard deviation or sigma parameter.
 *
 * @return Standard deviation.
 */
double NormalTrafficGenerator::getStandardDeviation() const {
	return standardDeviation;
}

/**
 * @brief Creates an instance of Normal traffic event with PDU.
 *
 * @param pduSize Size of PDU to generate.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> NormalTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, bool recordRoute) {
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
		scheduler.schedule(Event(generateNormalVariate(), eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of Normal traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param pduContents Reference to Entity object that will be carried by the token.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> NormalTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> pduContents,
																						   bool recordRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(pduContents, recordRoute);
	if (token == nullptr) {
		return nullptr;
	}
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(token, pduSize));
	//std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(pduContents, recordRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(generateNormalVariate(), eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of Normal traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> NormalTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute,
																						   bool recordRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute, recordRoute);
	if (token == nullptr) {
		return nullptr;
	}
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(token, pduSize));
	//std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(explicitRoute, recordRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(generateNormalVariate(), eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of Normal traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> NormalTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents,
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
		scheduler.schedule(Event(generateNormalVariate(), eventType, pdu));
	}
	return pdu;
}