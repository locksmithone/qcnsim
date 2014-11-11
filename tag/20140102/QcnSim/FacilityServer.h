/**
 * @author Marcos Portnoi
 * @date May 2013
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
#include <string>
#include <memory>

/**
 * @brief Facility Server class.
 * 
 * @par Description
 * This class models a server within a facility.
 */
class FacilityServer {
	//unsigned int id;					//!< Server ID.
	bool isBusy;						//!< True if busy; false if available.
	std::shared_ptr<const Token> token;	//!< Token currently being processed at this server.
	unsigned int releasedTokensCount;   //!< Count of tokens served by this server.
	double serviceStartTime;			//!< Absolute time at which processing has started.
	double sumBusyTime;					//!< Sum (integral) of time server was busy up to this moment.

//public:
	FacilityServer();

	friend class Facility;
};			