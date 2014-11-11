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

#include "Event.h"

/**
 * Constructor with parameters.
 * 
 * @param occurAfterTime Occurrence latency, i.e., event occurs after occurAfterTime time.
 * @param eventType Type of the event.
 * @param entity Simulator entity object associated with event.
 */
Event::Event(double occurAfterTime, EventType eventType, std::shared_ptr<const Entity> entity): occurAfterTime(occurAfterTime), eventType(eventType), entity(entity) { /// @todo Who deletes entity?
}

/**
 * @brief Default constructor.
 */
Event::Event() {
}

/**
 * Operator == (equal) overloading.
 *
 * @details 
 * This is the overloading of the comparison == operator for use with Event.  The result should be TRUE
 * if the left Event has the same contents as the right Event.
 * 
 * @param left The Event object to the "left", to be compared with the "right".
 * @param right The Event object to the "right", to be compared with the "left".
 */
bool operator==(const Event &left, const Event &right)  {
	return (left.entity == right.entity && left.eventType == right.eventType && left.occurAfterTime == right.occurAfterTime); 
}

/**
 * Operator != (not equal) overloading.
 *
 * @details 
 * This is the overloading of the comparison != operator for use with Event.  The result should be FALSE
 * if the left Event has the same contents as the right Event.
 *
 * Note that this function is simple the negation of == operator. It is being implemented for the sake of 
 * fundamental rule of operator overloading.
 *
 * @param left The Event object to the "left", to be compared with the "right".
 * @param right The Event object to the "right", to be compared with the "left".
 */
bool operator!=(const Event &left, const Event &right)  {
	return !operator==(left, right);
}