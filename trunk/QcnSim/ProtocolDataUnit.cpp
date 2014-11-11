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

#include "ProtocolDataUnit.h"

/**
 * @brief Constructor with parameters, but previous and next.
 *
 * @details 
 * ID is provided by parameter. AbsoluteGenerationTime is marked as invalid by putting max value of double (since to mark the timestamp, simulatorGlobals object is needed.
 * 
 * @param id PDU unique ID.
 * @param priority PDU priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this PDU.
 * @param destination Destination entity of this PDU.
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames. 
 */
ProtocolDataUnit::ProtocolDataUnit(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, unsigned int pduSize): 
		Token(id, priority, associatedEntity, source, destination), pduSize(pduSize), ttl(DEFAULT_TTL) {
}

/**
 * @brief Constructor with parameters.
 * 
 * @details 
 * ID is provided by parameter. AbsoluteGenerationTime is marked as invalid by putting max value of double (since to mark the timestamp, simulatorGlobals object is needed.
 *
 * @param id PDU unique ID.
 * @param priority PDU priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this PDU.
 * @param destination Destination entity of this PDU.
 * @param previous Previous entity that had this PDU (for PDU routing).
 * @param next Next entity that will have to process this PDU (to which entity the token has to be "sent", e.g., for which request service).
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames. 
 */
ProtocolDataUnit::ProtocolDataUnit(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, unsigned int pduSize): 
		Token(id, priority, associatedEntity, source, destination, previous, next), pduSize(pduSize), ttl(DEFAULT_TTL) {
}

/**
 * @brief Constructor with parameters (but previous and next), automatic PDU ID creation.
 *
 * @details 
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic PDU ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic PDU ID will be obtained.
 * @param priority PDU priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this PDU.
 * @param destination Destination entity of this PDU.
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames. 
 */
ProtocolDataUnit::ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, unsigned int pduSize): 
		Token(simulatorGlobals, priority, associatedEntity, source, destination), pduSize(pduSize), ttl(DEFAULT_TTL) {
}

/**
 * @brief Constructor with parameters, automatic PDU ID creation.
 *
 * @details 
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic PDU ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic PDU ID will be obtained.
 * @param priority PDU priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this PDU.
 * @param destination Destination entity of this PDU.
 * @param previous Previous entity that had this PDU (for PDU routing).
 * @param next Next entity that will have to process this PDU (to which entity the PDU has to be "sent", e.g., for which request service).
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames. 
 */
ProtocolDataUnit::ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, unsigned int pduSize): 
		Token(simulatorGlobals, priority, associatedEntity, source, destination, previous, next), pduSize(pduSize), ttl(DEFAULT_TTL) {
}

/**
 * @brief Constructor with parameters (but previous and next), automatic PDU ID creation.
 *
 * @details 
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic PDU ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic PDU ID will be obtained.
 * @param priority PDU priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this PDU.
 * @param destination Destination entity of this PDU.
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 */
ProtocolDataUnit::ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute): 
		Token(simulatorGlobals, priority, associatedEntity, source, destination, explicitRoute), pduSize(pduSize), ttl(DEFAULT_TTL) {
}

/**
 * @brief Constructor with parameters, automatic PDU ID creation.
 *
 * @details 
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic PDU ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic PDU ID will be obtained.
 * @param priority PDU priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this PDU.
 * @param destination Destination entity of this PDU.
 * @param previous Previous entity that had this PDU (for PDU routing).
 * @param next Next entity that will have to process this PDU (to which entity the PDU has to be "sent", e.g., for which request service).
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 */
ProtocolDataUnit::ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute): 
		Token(simulatorGlobals, priority, associatedEntity, source, destination, previous, next, explicitRoute), pduSize(pduSize), ttl(DEFAULT_TTL) {
}

/**
 * @brief Constructor that takes a Token and pduSize as parameter.
 *
 * @details 
 * This constructor copies all fields form a Token into a new PDU, and adds pduSize.
 * Note that if the token is nullptr, then an exception will be thrown here.
 * 
 * @param token Already existing Token object to be copied.
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames.
 */
ProtocolDataUnit::ProtocolDataUnit(std::shared_ptr<Token> token, unsigned int pduSize): Token(token->id, token->priority, token->associatedEntity,
																							  token->source, token->destination) {
	previous = token->previous;
	next = token->next;
	this->pduSize = pduSize;
	this->setExplicitRoute(token->getExplicitRoute());
	ttl = DEFAULT_TTL;
	absoluteGenerationTime = token->getAbsoluteGenerationTime();
	recordThisRoute = token->isRouteBeingRecorded();
}

/**
 * @brief Constructor that takes a Token and pduSize as parameter.
 *
 * @details 
 * This constructor copies all fields form a Token into a new PDU, and adds pduSize.
 * Note that if the token is nullptr, then an exception will be thrown here.
 * 
 * @param token Already existing Token object to be copied.
 * @param pduSize Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 */
ProtocolDataUnit::ProtocolDataUnit(std::shared_ptr<Token> token, unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute): Token(token->id,
																																				  token->priority,
																																				  token->associatedEntity,
																																				  token->source,
																																				  token->destination) {
	previous = token->previous;
	next = token->next;
	this->pduSize = pduSize;
	this->setExplicitRoute(explicitRoute);
	ttl = DEFAULT_TTL;
	absoluteGenerationTime = token->getAbsoluteGenerationTime();
	recordThisRoute = token->isRouteBeingRecorded();
}

/**
 * @brief Decrement TTL value.
 *
 * @details 
 * Decrements the TTL value by one. Typically used by a hop when receiving a PDU.
 */
void ProtocolDataUnit::decrementTtl() {
	--ttl;
}
		

/**
 * @brief Gets current TTL value.
 *
 * @return TTL value.
 */
unsigned short int ProtocolDataUnit::getTtl() const {
	return ttl;
}

/**
 * @brief Sets TTL (Time To Live) parameter.
 *
 * @details 
 * The Time To Live controls the lifetime of a PDU. At each hop, the TTL is decremented. Once the TTL reaches
 * zero, the hop (or router) will discard the PDU without forwarding it or passing it to upper layers.
 * The TTL is a simple solution to prevent cycles in a network.
 *
 * @param ttl TTL value.
 */
void ProtocolDataUnit::setTtl(unsigned short int ttl) {
	this->ttl = ttl;
}		
		
/**
 * @brief Sets PDU size in bytes.
 *
 * @param pduSize Size of PDU in bytes.
 */
void ProtocolDataUnit::setPduSize(unsigned int pduSize) {
	this->pduSize = pduSize;
}		

/**
 * @brief Gets PDU size or length in bytes.
 *
 * @return Size in bytes.
 */
unsigned int ProtocolDataUnit::getPduSize() const {
	return pduSize;
}


///**
// * Operator == (comparison) overloading.
// *
// * @details 
// * This is the overloading of the comparison operator for use with PDU.  The result should be TRUE
// * if the left PDU has the same contents as the right PDU.
// * 
// * @param left The PDU object to the "left", to be compared with the "right".
// * @param right The PDU object to the "right", to be compared with the "left".
// */
//bool operator==(const ProtocolDataUnit &left, const ProtocolDataUnit &right)  {
//	return (left.id == right.id && left.priority == right.priority && left.associatedEntity == right.associatedEntity &&
//		left.previous == right.previous && left.next == right.next && left.source == right.source && left.destination == right.destination &&
//		left.absoluteGenerationTime == right.absoluteGenerationTime);
//}
