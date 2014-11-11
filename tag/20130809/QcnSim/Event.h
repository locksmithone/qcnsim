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

#include "EventType.h"
#include "Entity.h"
#include <memory>

/**
 * @brief Event class
 * 
 * @par Description
 * The Event class is parameterized for generic objects (the <T>) and implements
 * an Event for the simulator. An Event has a latency field, or the amount of
 * time it will take for the event to initiate from the current time or clock
 * time; a field for the type of the event; and a generic object associated with
 * the event.
 */
class Event {
public:
	double occurAfterTime; //!< Occurrence latency, i.e., event occurs after occurAfterTime time.
    EventType eventType; //!< Type of the event.
    std::shared_ptr<const Entity> entity; //!< Simulator entity object associated with event.
	
	/**
	 * Constructor with parameters.
	 * 
	 * @param occurAfterTime Occurrence latency, i.e., event occurs after occurAfterTime time.
	 * @param event Type of the event.
	 * @param entity Simulator entity object associated with event.
	 */
	Event(double occurAfterTime, EventType eventType, std::shared_ptr<const Entity> entity); ///@todo Who deletes entity?

	Event();

	///	Comparator ==
	friend bool Event::operator==(const Event &left, const Event &right);
};