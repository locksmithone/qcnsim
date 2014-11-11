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

#include "QcnSensorTrafficGenerator.h"

/**
 * Constructor with parameter.
 *
 * @details 
 * This generator resembles the QCN sensors created in QCN Explorer. Typically, a QCN sensor traffic generator will be
 * triggered by a seismic event and will create one PDU message to be sent to its destination BOINC server or node.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, necessary to fullfil TraffiGenerator constructor reference parameter.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param latitude Latitude of this sensor.
 * @param longitude Longitude of this sensor.
 * @param qcnExplorerSensorId Sensor ID as assigned by QCNExplorer.
 * @param regionId Region ID as assigned by QCNExplorer.
 */
QcnSensorTrafficGenerator::QcnSensorTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double latitude, double longitude,
		unsigned int qcnExplorerSensorId, unsigned int regionId): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority), latitude(latitude), longitude(longitude), qcnExplorerSensorId(qcnExplorerSensorId),
		regionId(regionId) {
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> QcnSensorTrafficGenerator::createInstanceTrafficEvent() {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent();
	if (token != nullptr) {
		scheduler.schedule(Event(0.0, eventType, token));
	}
	return token;
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> QcnSensorTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents);
	if (token != nullptr) {
		scheduler.schedule(Event(0.0, eventType, token));
	}
	return token;
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> QcnSensorTrafficGenerator::createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(0.0, eventType, token));
	}
	return token;
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> QcnSensorTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute);
	if (token != nullptr) {
		scheduler.schedule(Event(0.0, eventType, token));
	}
	return token;
}

/**
 * @brief Get Latitude.
 *
 * @return Latitude of sensor.
 */
double QcnSensorTrafficGenerator::getLatitude() const {
	return latitude;
}

/**
 * @brief Get Longitude.
 *
 * @return Longitude of sensor.
 */
double QcnSensorTrafficGenerator::getLongitude() const {
	return longitude;
}

/**
 * @brief Get QCN Explorer Sensor ID.
 *
 * @return Sensor ID as assigned by QCN Explorer.
 */
unsigned int QcnSensorTrafficGenerator::getQcnExplorerSensorId() const {
	return qcnExplorerSensorId;
}

/**
 * @brief Get Region ID.
 *
 * @return Region ID as assigned by QCN Explorer.
 */
unsigned int QcnSensorTrafficGenerator::getRegionId() const {
	return regionId;
}

/**
 * @brief Set Latitude.
 *
 * @param latitude Latitude of sensor.
 */
void QcnSensorTrafficGenerator::setLatitude(double latitude) {
	this->latitude = latitude;
}

/**
 * @brief Set Longitude.
 *
 * @param longitude Longitude of sensor.
 */
void QcnSensorTrafficGenerator::setLongitude(double longitude) {
	this->longitude = longitude;
}

/**
 * @brief Set QCN Explorer Sensor Id.
 *
 * @param qcnExplorerSensorId QCN Explorer Sensor ID.
 */
void QcnSensorTrafficGenerator::setQcnExplorerSensorId(unsigned int qcnExplorerSensorId) {
	this->qcnExplorerSensorId = qcnExplorerSensorId;
}

/**
 * @brief Set Region ID.
 *
 * @param regionId Region ID.
 */
void QcnSensorTrafficGenerator::setRegionId(unsigned int regionId) {
	this->regionId = regionId;
}

/**
 * @brief Creates an instance of QCN sensor traffic event with PDU.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param pduSize Size of PDU to generate.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns nullptr.
 */
std::shared_ptr<ProtocolDataUnit> QcnSensorTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(0.0, eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param pduSize Size of PDU to generate.
 * @param pduContents Reference to Entity object that will be carried by the token.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<ProtocolDataUnit> QcnSensorTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> pduContents) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(pduContents), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(0.0, eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param pduSize Size of PDU to generate.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<ProtocolDataUnit> QcnSensorTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(explicitRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(0.0, eventType, pdu));
	}
	return pdu;
}

/**
 * @brief Creates an instance of QCN sensor traffic event.
 *
 * @details  
 * Calls parent function and complements with event generation.
 * Uses member variable tokenContents.
 * The instance is scheduled to occur immediately, i.e., at 0.0 delay.
 *
 * @param pduSize Size of PDU to generate.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return PDU that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<ProtocolDataUnit> QcnSensorTrafficGenerator::createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents,
																						   std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a PDU and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<ProtocolDataUnit> pdu(new ProtocolDataUnit(TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute), pduSize));
	if (pdu != nullptr) {
		scheduler.schedule(Event(0.0, eventType, pdu));
	}
	return pdu;
}
