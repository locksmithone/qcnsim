/**
 * @author Marcos Portnoi
 * @date July 2013
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

#include "TrafficGenerator.h"

/**
 * Constructor with parameter.
 *
 * @details 
 * The seed for the random number generator is controlled by SimulatorGlobals. If none is ever specified, random_device output is used as seed.
 * The generator is constructed off by default.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, to get random generator info.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 */
TrafficGenerator::TrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority): scheduler(scheduler), simulatorGlobals(simulatorGlobals), eventType(eventType),
		tokenContents(tokenContents), source(source), destination(destination), priority(priority), isOn(false), tokensGeneratedCount(0) {
}

/**
 * Constructor with parameters, seed as parameter.
 * 
 * @details 
 * Seed is defined here from parameters. The generator is constructed off by default.
 *
 * @param simulatorGlobals Reference to SimulatorGlobals object, to get random generator info.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param seed Seed to the default random number generator, to be passed to SimulatorGlobals object.
 */
TrafficGenerator::TrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, unsigned int seed): scheduler(scheduler), simulatorGlobals(simulatorGlobals), eventType(eventType),
		tokenContents(tokenContents), source(source), destination(destination), priority(priority), isOn(false), tokensGeneratedCount(0) {
	simulatorGlobals.seedRandomNumberGenerator(seed);
}

/**
 * Abstract function to serve as template for child classes, to create an instance of traffic event.
 *
 * @details 
 * This primary implementation only creates the token. Child classes may call this function but implement the rest, such as creating an event,
 * and scheduling with the proper event time (generated from the appropriate probability distribution).
 *
 * October 2013: Tokens are generated with previous = next = source to facilitate Node forwarding.
 * 
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> TrafficGenerator::createInstanceTrafficEvent(bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	if (isOn) {
		std::shared_ptr<Token> token(new Token(simulatorGlobals, this->priority, this->tokenContents, this->source, this->destination, this->source, this->source));
		if (recordRoute) {
			token->setRecordThisRoute();
		}
		++tokensGeneratedCount; // One more token generated.
		return token;
	} else {
		return nullptr;
	}
}

/**
 * Abstract function to serve as template for child classes, to create an instance of traffic event with determined token contents.
 *
 * @details 
 * This primary implementation only creates the token. Child classes may call this function but implement the rest, such as creating an event,
 * and scheduling with the proper event time (generated from the appropriate probability distribution).
 *
 * October 2013: Tokens are generated with previous = next = source to facilitate Node forwarding.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 * 
 * @return Token that was generated, per the class constructor parameters, and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> TrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals. Use tokenContents from parameters.
	if (isOn) {
		std::shared_ptr<Token> token(new Token(simulatorGlobals, this->priority, tokenContents, this->source, this->destination, this->source, this->source));
		if (recordRoute) {
			token->setRecordThisRoute();
		}
		++tokensGeneratedCount; // One more token generated.
		return token;
	} else {
		return nullptr;
	}
}

/**
 * Abstract function to serve as template for child classes, to create an instance of traffic event and explicit route.
 *
 * @details 
 * This primary implementation only creates the token. Child classes may call this function but implement the rest, such as creating an event,
 * and scheduling with the proper event time (generated from the appropriate probability distribution).
 *
 * October 2013: Tokens are generated with previous = next = source to facilitate Node forwarding.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 * 
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> TrafficGenerator::createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	if (isOn) {
		std::shared_ptr<Token> token(new Token(simulatorGlobals, this->priority, this->tokenContents, this->source, this->destination, this->source, this->source, explicitRoute));
		if (recordRoute) {
			token->setRecordThisRoute();
		}
		++tokensGeneratedCount; // One more token generated.
		return token;
	} else {
		return nullptr;
	}
}

/**
 * Abstract function to serve as template for child classes, to create an instance of traffic event with determined token contents and explicit route.
 *
 * @details 
 * This primary implementation only creates the token. Child classes may call this function but implement the rest, such as creating an event,
 * and scheduling with the proper event time (generated from the appropriate probability distribution).
 *
 * October 2013: Tokens are generated with previous = next = source to facilitate Node forwarding.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 * 
 * @return Token that was generated, per the class constructor parameters, and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<Token> TrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals. Use tokenContents from parameters.
	if (isOn) {
		std::shared_ptr<Token> token(new Token(simulatorGlobals, this->priority, tokenContents, this->source, this->destination, this->source, this->source, explicitRoute));
		if (recordRoute) {
			token->setRecordThisRoute();
		}
		++tokensGeneratedCount; // One more token generated.
		return token;
	} else {
		return nullptr;
	}
}

/**
 * Turns generator On.
 *
 */
void TrafficGenerator::turnOn() {
	this->isOn = true;
}

/**
 * Turns generator Off.
 *
 */
void TrafficGenerator::turnOff() {
	this->isOn = false;
}

/**
 * Set token contents.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 */
void TrafficGenerator::setTokenContents(std::shared_ptr<Entity> tokenContents) {
	this->tokenContents = tokenContents;
}

/**
 * Get token contents.
 *
 * @return Token contents included in tokens generated by this class.
 */
std::shared_ptr<Entity> TrafficGenerator::getTokenContents() const {
	return tokenContents;
}

/**
 * Get EventType this generator includes in Tokens.
 *
 * @return EventType.
 */
EventType TrafficGenerator::getEventType() const {
	return eventType;
}

/**
 * Set EventType this generator includes in Tokens.
 *
 * @param eventType EventType to include in tokens generated by this generator.
 */
void TrafficGenerator::setEventType(EventType eventType) {
	this->eventType = eventType;
}

/**
 * Get source Entity this generator includes in Tokens.
 *
 * @return Source Entity.
 */
std::shared_ptr<Entity> TrafficGenerator::getSource() const {
	return source;
}

/**
 * Set source Entity this generator includes in Tokens.
 *
 * @param source Source Entity to include in tokens generated by this class.
 */
void TrafficGenerator::setSource(std::shared_ptr<Entity> source) {
	this->source = source;
}

/**
 * Get destination for tokens generated from this generator..
 *
 * @return Destination facility.
 */
std::shared_ptr<Entity> TrafficGenerator::getDestination() const {
	return destination;
}

/**
 * Get value of counter for tokens (or traffic events) generated.
 *
 * @return Number of tokens generated so far by this traffic generator.
 */
unsigned int TrafficGenerator::getTokensGeneratedCount() const {
	return this->tokensGeneratedCount;
}

/**
 * Set destination for tokens generated from this generator..
 *
 * @param destination Destination facility.
 */
void TrafficGenerator::setDestination(std::shared_ptr<Entity> destination) {
	this->destination = destination;
}

/**
 * @brief Creates an instance of traffic event with PDU.
 *
 * @param pduSize Size of PDU to generate.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> TrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, bool recordRoute) {
	if (isOn) {
		std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(createInstanceTrafficEvent(recordRoute), pduSize));
		return pdu;
	} else {
		return nullptr;
	}
}

/**
 * @brief Creates an instance of traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param pduContents Reference to Entity object that will be carried by the token.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> TrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> pduContents, bool recordRoute) {
	if (isOn) {
		std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(createInstanceTrafficEvent(pduContents, recordRoute), pduSize));
		return pdu;
	} else {
		return nullptr;
	}
}

/**
 * @brief Creates an instance of traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> TrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute,
																				  bool recordRoute) {
	if (isOn) {
		std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(createInstanceTrafficEvent(explicitRoute, recordRoute), pduSize));
		return pdu;
	} else {
		return nullptr;
	}
}

/**
 * @brief Creates an instance of traffic event.
 *
 * @param pduSize Size of PDU to generate.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 * @param recordRoute True if this generated token should record the route it follows. False otherwise. Default is false.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> TrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents,
																						   std::vector<std::shared_ptr<Entity>> explicitRoute,
																						   bool recordRoute) {
	if (isOn) {
		std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(createInstanceTrafficEvent(tokenContents, explicitRoute, recordRoute), pduSize));
		return pdu;
	} else {
		return nullptr;
	}
}