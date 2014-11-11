/**
 * @author Marcos Portnoi
 * @date June 2013
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

#include "Entity.h"
#include <string>

/**
 * @brief Message class.
 * 
 * @par Description
 * Messages are anything relevant that should be passed between elements of a simulation, such as packet contents between hosts,
 * XML files between clients and servers, etc. Typically, a Message object will be the contents of a token or PDU.
 */
class Message: public Entity {
private:
	std::string contents;

public:
	Message();
	explicit Message(const std::string &contents);

	std::string getContents() const;
	void setContents(const std::string &contents);
};