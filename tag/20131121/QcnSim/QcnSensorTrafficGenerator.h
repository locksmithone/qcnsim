/**
* @author Marcos Portnoi
* @date October 2013
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

#pragma once

#include "TrafficGenerator.h"
#include "EventType.h"
#include "ProtocolDataUnit.h"
#include <memory>

/**
* @brief QCN Sensor Traffic Generator.
*
* @par Description
* This class implements a Quake-Catcher Network Sensor Traffic Generator.
* Basically, a simulated sensor will be triggered by a seismic event, and then the sensor will produce a PDU to be sent to a
* BOINC server. This PDU may contain information such as time of observed event, magnitude, and other sensor-related information.*
*/
class QcnSensorTrafficGenerator: public TrafficGenerator {
private:
	double latitude;  //!< Latitude of this sensor.
	double longitude; //!< Longitude of this sensor.
	unsigned int qcnExplorerSensorId; //!< Sensor ID as assigned by QCNExplorer.
	unsigned int regionId; //!< Region ID as assigned by QCNExplorer.

public:
	QcnSensorTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
								 std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double latitude, double longitude,
								 unsigned int qcnExplorerSensorId = 0, unsigned int regionId = 0);

	std::shared_ptr<Token> createInstanceTrafficEvent() override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute) override;
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize);
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents);
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute);
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute);
	
	double getLatitude() const;
	double getLongitude() const;
	unsigned int getQcnExplorerSensorId() const;
	unsigned int getRegionId() const;
	void setLatitude(double latitude);
	void setLongitude(double longitude);
	void setQcnExplorerSensorId(unsigned int qcnExplorerSensorId);
	void setRegionId(unsigned int regionId);
};
