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

#include "FacilityQueueElement.h"

/**
 * @brief Defines an element for the Event Chain.
 * 
 * @par Description
 * The Event Chain is a time-ordered list of events. The Scheduler is the agent
 * responsible for putting events into the list, ordered by occurrence in time,
 * and for retrieving events from the beginning of the list. When the Scheduler
 * retrieves an event at the head of the list, it "causes" the event, or
 * activates the agents responsible for "executing" the actions predicted in the
 * event (such as a message arrival, message sending, etc.).
 */

/**
 * Constructor with parameters.
 *
 * @param token Token object.
 * @param eventType Eventy type associated with token.
 * @param serviceTime Service time of this token.  It can be the remaining service time if the token
 *					  was in service and was preempted (thus sent back to queue). For blocked tokens
 *			          (i.e., tokens to which service did not yet begin), use serviceTime = 0.0.left The FacilityQueueElement object to the "left", to be compared with the "right".
 */
FacilityQueueElement::FacilityQueueElement(std::shared_ptr<const Token> token, EventType eventType, double serviceTime): eventType(eventType), serviceTime(serviceTime) {
	this->token = token;
}

/**
 * Operator < overloading.
 *
 * @details
 * This is the overloading of the < operator for use with priority_queue container.  The result should be TRUE
 * if the left token has lower priority than the right token.
 * 
 * @param left The FacilityQueueElement object to the "left", to be compared with the "right".
 * @param right The FacilityQueueElement object to the "right", to be compared with the "left".
 */
bool operator<(const FacilityQueueElement &left, const FacilityQueueElement &right)  {
	return left.token->priority >= right.token->priority;
}
