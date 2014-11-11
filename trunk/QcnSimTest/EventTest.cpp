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

// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include "../gtest/include/gtest/gtest.h"
#include "../QcnSim/EventChainElement.h"
#include "../QcnSim/Event.h"
#include "../QcnSim/EventType.h"
#include <list>
#include <queue>

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

/// Tests the operator == and < overloading of EventChainElement. 
TEST(EventTest, EventChainElementOperator) {
	// Creates a few of events to test.
	Event eventFirst = Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0);
	Event eventSecond = Event(2.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, 0);
	Event eventFirstCopy = Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0);

	// Create a list of EventChainElement.
	std::list<EventChainElement> eventChain;
	
	// Create EventChainElement objects.
	EventChainElement elementFirst = EventChainElement(1.0, eventFirst);
	EventChainElement elementSecond = EventChainElement(3.0, eventSecond);
	EventChainElement elementFirstCopy = EventChainElement(1.0, eventFirstCopy);

	// Insert two events into this list, by creating EventChainElement objects. Put in reverse order.
	eventChain.push_back(elementSecond);
	eventChain.push_back(elementFirst);
	
	// Does the list have 2 elements?
	EXPECT_EQ(2, eventChain.size());

	// Is the head element the one that has to occur last (i.e., the order is wrong)?
	// Also tests overloaded == operator.
	EXPECT_EQ(elementSecond, eventChain.front());

	// Tests whether eventFirst and eventFirstCopy are really similar.
	EXPECT_TRUE(eventFirst == eventFirstCopy);

	// Tests whether elementFirst and elementFirstCopy are really similar.
	EXPECT_TRUE(elementFirst == elementFirstCopy);

	// Check whether elementFirst must occur before elementSecond.
	EXPECT_TRUE(elementFirst < elementSecond);

	// Check whether elementFirst must occur before or "at the same time" as elementSecond.
	EXPECT_TRUE(elementFirst <= elementSecond);

	// Check whether elementFirst *does not* occur before elementFirstCopy.
	EXPECT_FALSE(elementFirst < elementFirstCopy);
}

/// Tests priority queue. The < operator overloading does the opposite here...
TEST(EventTest, PriorityQueue) {
	// Creates a few of events to test.
	Event eventFirst = Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0);
	Event eventSecond = Event(2.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, 0);
	Event eventFirstCopy = Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0);

	// Create a list of EventChainElement.
	std::priority_queue<EventChainElement> eventChain;
	
	// Create EventChainElement objects.
	EventChainElement elementFirst = EventChainElement(1.0, eventFirst);
	EventChainElement elementSecond = EventChainElement(3.0, eventSecond);
	EventChainElement elementFirstCopy = EventChainElement(1.0, eventFirstCopy);

	// Insert two events into this list, by creating EventChainElement objects. Put in reverse order.
	eventChain.push(elementFirst);
	eventChain.push(elementSecond);
	
	// Does the list have 2 elements?
	EXPECT_EQ(2, eventChain.size());

	// Is the head element the one that has to occur last (i.e., the order is wrong)?
	// It should, since operator < overloading must be "inverted", here, for priority_queue container to work properly.
	// Also tests overloaded == operator.
	EXPECT_EQ(elementSecond, eventChain.top());

	// Is the head element, elementFirst? It should not, since operator < overloading is not being "inverted" for priority_queue container.
	EXPECT_FALSE(elementFirst == eventChain.top());

}