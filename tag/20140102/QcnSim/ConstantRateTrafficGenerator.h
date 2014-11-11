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

#pragma once

#include "TrafficGenerator.h"
#include "EventType.h"
#include "ProtocolDataUnit.h"
#include <memory>

/**
 * @brief ConstantRateTrafficGenerator class.
 * 
 * @par Description
 * This class implements a constant-rate traffic generator (or a CBR - Constant Bit Rate generator).
 * Events generated from this class will be distributed in time according to a constant, fixed number.
 *
 */
class ConstantRateTrafficGenerator: public TrafficGenerator {
private:
	double interval;  //!< Fixed, constant time interval for this generator.
	
public:
	ConstantRateTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double interval);

	std::shared_ptr<Token> createInstanceTrafficEvent(bool recordRoute = false) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, bool recordRoute = false) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute = false) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute = false) override;
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, bool recordRoute = false) override;
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents, bool recordRoute = false) override;
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute = false) override;
	std::shared_ptr<ProtocolDataUnit> createInstanceTrafficEventPdu(unsigned int pduSize, std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute, bool recordRoute = false) override;
	double getInterval() const;
};
