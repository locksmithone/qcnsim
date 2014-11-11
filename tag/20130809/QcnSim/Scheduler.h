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

#include "EventChainElement.h"
#include "SimulatorGlobals.h"
#include "Entity.h"
#include <list>
#include <iostream>

/**
 * @brief Scheduler class.
 * 
 * @par Description
 * A simulator, in contrast with a real-time application or an emulator, works
 * in simulated time. It simulates the passage of real time in a generally
 * faster fashion. In event-driven simulators, this is achieved by the
 * generation of events that occur in specific times in the future. For example,
 * an event "movement" might be scheduled to occur now for a determined J-bug,
 * with the effect of the J-bug reaching the destination after 3 time units in
 * the future. If nothing else happens in the meantime, from the point of view
 * of the simulator, there is no reason to simulate the passage of time between
 * now and 3 units of time in the future; the simulator will just then "jump" to
 * the event "arrival at destination" 3 units of time in the future.
 * 
 * In effect, an event-driven simulation occurs by scheduling events that happen
 * in time and putting these events into the Event Chain. The Event Chain is a
 * time-ordered list of events. The Scheduler is the entity responsible for
 * putting events into the list, ordered by occurrence in time, and for
 * retrieving events from the beginning of the list. When the Scheduler
 * retrieves an event at the head of the list, it "causes" the event, or
 * activates the agents responsible for "executing" the actions predicted in the
 * event (such as an arrival, which can be described as the placement of the
 * J-bug in the expected arrival position).
 * 
 * For a simulation to start properly, there has to be at least one initial
 * event in the Event Chain. This initial event will cause (schedule) others
 * (thus, a chain), and the simulation will proceed by causing events in the
 * Event Chain. If there is no more events in the Event Chain, then the
 * simulation must halt. The simulation can also be stopped according to other
 * criteria, such as by reaching a target simulation time. * 
 */
class Scheduler {
private:
	/**
	* Not possible to use priority_queue, since elements have to be accessed iteratively to be removed (in case events in the future have to be removed).
	* Will have to use a list and make_heap, push_heap, pop_heap, etc. to maintain heap property.
	*/
	//std::priority_queue<EventChainElement> eventChain; //!<The Event Chain as a min-heap ordered by eventTime.
	std::list<EventChainElement> eventChain; //!<The Event Chain as a min-heap ordered by eventTime. Underlying container is a list, and this class should maintain heap order.
	SimulatorGlobals &simulatorGlobals;  //!< Reference to SimulatorGlobals object, to control simulation clock time and etc.

public:
	Scheduler(SimulatorGlobals &simulatorGlobals, const Event &event);
	Scheduler(SimulatorGlobals &simulatorGlobals);

	void schedule(const Event &event);
	void scheduleFront(const Event &event);
	Event cause();
	unsigned int removeEvents(std::shared_ptr<const Entity> entity);
	std::list<EventChainElement>::size_type getChainSize() const;
};

