/**
 * @author Marcos Portnoi
 * @date April 2013
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

#include "EventChainElement.h"

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
 * @param eventTime Absolute occurrence time of event (= current time + occurAfterTime of event).
 * @param event  Event object.
 */
EventChainElement::EventChainElement(double eventTime, const Event &event): eventTime(eventTime), event(event) {
}

/**
 * Operator < overloading.
 *
 * @details
 * This is the overloading of the < operator.  The result should be TRUE
 * if the left eventTime is earlier than the right eventTime.
 * 
 * @param left The EventChainElement object to the "left", to be compared with the "right".
 * @param right The EventChainElement object to the "right", to be compared with the "left".
 */
bool operator<(const EventChainElement &left, const EventChainElement &right)  {
	return left.eventTime < right.eventTime;
	// For priority_queue container, this should actually be true if left.eventTime >= right.eventTime.
	// But then the operator < breaks if a real < comparison is wanted.
}

/**
 * Operator <= overloading.
 *
 * @details
 * This is the overloading of the <= operator.  The result should be TRUE
 * if the left eventTime is earlier or equal than the right eventTime.
 * 
 * @param left The EventChainElement object to the "left", to be compared with the "right".
 * @param right The EventChainElement object to the "right", to be compared with the "left".
 */
bool operator<=(const EventChainElement &left, const EventChainElement &right)  {
	return left.eventTime <= right.eventTime;
}

/**
 * Operator == (comparison) overloading.
 *
 * @details
 * This is the overloading of the comparison operator for use with EventChainElement.  The result should be TRUE
 * if the left EventChainElement has the same contents as the right EventChainElement.
 * 
 * @param left The EventChainElement object to the "left", to be compared with the "right".
 * @param right The EventChainElement object to the "right", to be compared with the "left".
 */
bool operator==(const EventChainElement &left, const EventChainElement &right)  {
	return (left.event == right.event && left.eventTime == right.eventTime); 
}


