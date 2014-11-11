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

#include "Token.h"

#define DEFAULT_TTL 128 // Default, hard-coded TTL.

/**
 * @brief Protocol Data Unit class.
 * 
 * @par Description
 * This class inherits from Token class and models a PDU or Protocol Data Unit. This class will extend Token and add features for computer networks,
 * such as TTL (Time To Live) and size.
 */
class ProtocolDataUnit: public Token {
private:
	unsigned short int ttl; //!< Time to Live for this PDU. Default will be 128.
	unsigned int pduSize; //!< Size or length of this PDU, typically in bytes. Notice that unsigned short int is not being used so as to accomodate for (really) jumbo frames.  

public:
	ProtocolDataUnit(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, unsigned int pduSize);
	ProtocolDataUnit(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, unsigned int pduSize);
	ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, unsigned int pduSize);
	ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, unsigned int pduSize);
	ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute);
	ProtocolDataUnit(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute);
	ProtocolDataUnit(std::shared_ptr<Token> token, unsigned int pduSize);
	ProtocolDataUnit(std::shared_ptr<Token> token, unsigned int pduSize, std::vector<std::shared_ptr<Entity>> explicitRoute);

	unsigned short int getTtl() const;
	unsigned int getPduSize() const;
	void setTtl(unsigned short int ttl);
	void setPduSize(unsigned int pduSize);
	void decrementTtl();

	///	Comparator
	//friend bool ProtocolDataUnit::operator==(const ProtocolDataUnit &left, const ProtocolDataUnit &right);

};					