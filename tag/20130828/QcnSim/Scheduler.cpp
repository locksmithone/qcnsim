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

#include "Scheduler.h"

/**
 * @brief Constructor with initial event.
 *
 * @details
 * Calls function schedule to schedule this first event.
 * 
 * @param simulatorGlobals SimulatorGlobals object.
 * @param event New event to insert.
 */
Scheduler::Scheduler(SimulatorGlobals &simulatorGlobals, const Event &event): simulatorGlobals(simulatorGlobals) {
    // This is the first event, so just add it to the heap.
	schedule(event);
}

/**
 * Constructor without initial event.
 *
 * @details
 * No initial event, just the SimulatorGlobals reference.
 * 
 * @param simulatorGlobals SimulatorGlobals object.
 */
Scheduler::Scheduler(SimulatorGlobals &simulatorGlobals): simulatorGlobals(simulatorGlobals) {
}

/**
 * Schedules an event in time, ordered by time.
 * 
 * @details
 * The event occurrence time is calculated by adding currentAbsoluteTime and event's occurAfterTime.
 * Adds element to the list ordered in time, after events with same absolute occurrence time.
 * 
 * @param event New event to insert.
 */
void Scheduler::schedule(const Event &event) {
	// Search for insertion point within the event chain, i.e., look for event (in event chain) just after new event to be inserted.
	double eventTime = simulatorGlobals.getCurrentAbsoluteTime() + event.occurAfterTime; // Absolute occurrence time
	std::list<EventChainElement>::iterator eventChainIterator = eventChain.begin();
	while (eventChainIterator != eventChain.end()) {
		if (eventChainIterator->eventTime <= eventTime) { // Looking for event with eventTime *after* the new event to insert
			++eventChainIterator;
		} else {
			// Found next event after new event to be inserted. Insert new event before.
			eventChain.insert(eventChainIterator, EventChainElement(eventTime, event)); // Insert element (new EventChainElement with copy of event).
			return; // Done.
		}
	}
	// Can only reach this point if end of event chain was reached, i.e., the new event must be inserted at the end of event chain.
	eventChain.push_back(EventChainElement(eventTime, event));
}

/**
 * Schedules an event in front of the event chain.
 * 
 * @details
 * The event occurrence time is equal to currentAbsoluteTime, since this event must occur before any other event.
 * This is a function tailored for dequeued tokens from facilities, which have to have service request scheduled for them before anything else happens in the simulation.
 * Misuse of this function may cause unsound simulations.
 * 
 * @param event New event to insert.
 */
void Scheduler::scheduleFront(const Event &event) {
	eventChain.push_front(EventChainElement(simulatorGlobals.getCurrentAbsoluteTime(), event));
}

/**
 * Removes and returns the first event in the Event Chain.
 *
 * @details
 * Advances the SimulatorGlobals.currentAbsoluteTime to the eventTime (event absolute occurrence time).
 * 
 * @return Next event to cause.
 */
Event Scheduler::cause() {
    try {
		simulatorGlobals.setCurrentAbsoluteTime((eventChain.begin())->eventTime); // Sets currentAbsoluteTime to first event's eventTime (advances or jumps the clock).
		// Removes and returns the first event.
		Event nextEvent = eventChain.front().event;
		eventChain.pop_front(); ///@todo Will this invalidate nextEvent? Is nextEvent a copy?
		return nextEvent; 
    }
    catch (int e) {
        // Empty Event Chain.  Should not happen... should always have at least an END_SIMULATION event.
		std::cout << "Event Chain is empty.  No more events to process, ending simulation..." << std::endl;
        exit(e);
        //return NULL;
    }
}

/**
 * Removes events from Event Chain that contains a matching entity in the Event object.
 * 
 * @param entity Entity to seek and remove corresponding events from event chain.
 * @return Returns number of events removed; zero if no matching event was found.
 */
/// @todo Should this return the number of events deleted, or a list with the deleted events? Perhaps we still need the objects inside the events?
unsigned int Scheduler::removeEvents(std::shared_ptr<const Entity> entity) {
    // Find all eventChainElements in which Events contain Entity object, and remove them all.
	unsigned int removedEventsCounter = 0; // How many events were removed?
	// Using the regular iterator loop instead of C++11 foreach, since we need the iterator to remove elements anyway (or be forced to search
	// the list one more time to remove by value.
	// This will actually not work, since, when removing an element, the loop iterator pointing to it will be invalidated. Therefore,
	// cannot be incremented. See http://stackoverflow.com/questions/596162/can-you-remove-elements-from-a-stdlist-while-iterating-through-it.
	//for (std::list<EventChainElement>::iterator eventIterator = eventChain.begin(); eventIterator != eventChain.end(); ++eventIterator) {
	//	if (eventIterator->event.entity == entity) {
	//		// Found event with matching Entity: remove.
	//		eventChain.erase(eventIterator);
	//		++removedEventsCounter; // Increment removed events counter.
	//	}
	//}
	std::list<EventChainElement>::iterator eventIterator = eventChain.begin();
	while (eventIterator != eventChain.end()) {
		// If this event has the specified Entity object, then remove it.
		if (eventIterator->event.entity == entity) {
			eventIterator = eventChain.erase(eventIterator); /* Looks weird? Check http://stackoverflow.com/questions/596162/can-you-remove-elements-from-a-stdlist-while-iterating-through-it.
															  * Notice that erase deletes the element at eventIterator, and returns the iterator to the *next* element. eventIterator has to
															  * has to point to this next element, otherwise it will point to a no-longer existing element.
															  */
			++removedEventsCounter; // Increment removed events counter.
		} else {
			++eventIterator;
		}
	}
	return removedEventsCounter;
}

/**
 * Returns the current size of the event chain.
 *
 * @return Size of event chain, in size_type.
 */
std::list<EventChainElement>::size_type Scheduler::getChainSize() const {
	return eventChain.size();
}


