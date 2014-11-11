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

#include "Entity.h"
#include "Token.h"
#include "EventType.h"
#include <memory>

/**
 * @brief Facility Queue Element class.
 * 
 * @par Description
 * This class models an element within the priority queue within a facility.
 * I.e., a facility has a priority queue. The priority queue has elements, composed of
 * a token, the associated event type, and a service time for the token.
 * Facilities can only process tokens.
 */
class FacilityQueueElement {
	std::shared_ptr<const Token> token;	//!< Token object.
	EventType eventType;			//!< Eventy type associated with token.
	double serviceTime;				//!< Service time of this token.  It can be the remaining service time if the token
									//!< was in service and was preempted (thus sent back to queue). For blocked tokens
									//!< (i.e., tokens to which service did not yet begin), use serviceTime = 0.0.
	/// @todo Maybe we should include a flag "isBlocked" here and not fiddle with serviceTime = 0.0... It so happens that, if the token has just finished service, and
	/// @todo it is preempted before being released (serviceTime = 0.0), we need to set this serviceTime to > 0 (a very small positive number) so the processing works right.
	/// @todo Check SMPL. This is important for preempt function only.

//public:
	FacilityQueueElement(std::shared_ptr<const Token> token, EventType eventType, double serviceTime);

	/// Comparator, non-member
	friend bool operator<(const FacilityQueueElement &left, const FacilityQueueElement &right);

	friend class Facility; //!< Only Facility can access these private members.
};					