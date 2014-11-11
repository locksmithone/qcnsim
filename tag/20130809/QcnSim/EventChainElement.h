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

#pragma once

#include "Event.h"
//#include "Scheduler.h"  // No! Cyclic include!

/**
 * @brief Defines an element for the Event Chain.
 * 
 * @par Description
 * The Event Chain is a time-ordered list of events. The Scheduler is the entity
 * responsible for putting events into the list, ordered by occurrence in time,
 * and for retrieving events from the beginning of the list. When the Scheduler
 * retrieves an event at the head of the list, it "causes" the event, or
 * activates the agents responsible for "executing" the actions predicted in the
 * event (such as an arrival, which can be described as the placement of the
 * J-bug in the expected arrival position).
 */
class EventChainElement {
private:
	double eventTime;  //!< Absolute occurrence time of event (= current time + occurAfterTime of event).
	Event event;   //!< Event object.

public:
	/// Constructor
	EventChainElement(double eventTime, const Event &event);
	
	/// Operator <
	friend bool EventChainElement::operator<(const EventChainElement &left, const EventChainElement &right);

	/// Operator <=
	friend bool EventChainElement::operator<=(const EventChainElement &left, const EventChainElement &right);

	///	Operator ==
	friend bool EventChainElement::operator==(const EventChainElement &left, const EventChainElement &right);

	friend class Scheduler; // Only Scheduler can access these private members.
};