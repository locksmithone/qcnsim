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

#include "SchedulerTest.h"

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

/**
 * Constructor.
 *
 * Do initializations here.
 */
SchedulerTest::SchedulerTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "SchedulerTest")), 
		scheduler(Scheduler(simulatorGlobals)),
		eventFirst(Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0)),
		eventSecond(Event(2.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, 0)),
		eventThird(Event(3.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0)) {
}

/// Tests constructor, no initial event.
TEST_F(SchedulerTest, Constructor) {
	EXPECT_EQ(0, scheduler.getChainSize());
}

/// Schedules a few events.
TEST_F(SchedulerTest, ScheduleAll) {
	scheduler.schedule(eventFirst);
	EXPECT_EQ(1, scheduler.getChainSize());
	
	scheduler.schedule(eventSecond);
	EXPECT_EQ(2, scheduler.getChainSize());

	scheduler.schedule(eventThird);
	EXPECT_EQ(3, scheduler.getChainSize());
}

/// Schedules a few events with cause in between
TEST_F(SchedulerTest, ScheduleCauseInOrder) {
	scheduler.schedule(eventFirst);
	EXPECT_EQ(1, scheduler.getChainSize());

	Event causedEvent = scheduler.cause();
	// Is this really the first event?
	EXPECT_EQ(causedEvent, eventFirst);
	// Now the event chain must be empty.
	EXPECT_EQ(0, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(1.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Schedule second event.
	scheduler.schedule(eventSecond);
	// Must be the only one in chain.
	EXPECT_EQ(1, scheduler.getChainSize());
	// Cause it, check the event.
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventSecond);
	// Check simulation clock.
	EXPECT_EQ(3.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Schedule third event.
	scheduler.schedule(eventThird);
	// Cause it, check the event.
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventThird);
	// Check simulation clock.
	EXPECT_EQ(6.0, simulatorGlobals.getCurrentAbsoluteTime());

	// There should be an error here.
	// causedEvent = scheduler.cause();
}

/// Schedules a few events with cause in between; events are scheduled out of order
TEST_F(SchedulerTest, ScheduleCauseOutOfOrder) {
	scheduler.schedule(eventThird);
	EXPECT_EQ(1, scheduler.getChainSize());

	Event causedEvent = scheduler.cause();
	// Is this really the third event?
	EXPECT_EQ(causedEvent, eventThird);
	// Now the event chain must be empty.
	EXPECT_EQ(0, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(3.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Schedule first event.
	scheduler.schedule(eventFirst);
	// Must be the only one in chain.
	EXPECT_EQ(1, scheduler.getChainSize());
	// Cause it, check the event.
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFirst);
	// Check simulation clock.
	EXPECT_EQ(4.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Schedule second event.
	scheduler.schedule(eventSecond);
	// Cause it, check the event.
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventSecond);
	// Check simulation clock.
	EXPECT_EQ(6.0, simulatorGlobals.getCurrentAbsoluteTime());
}

/// Schedules all events at once, causes one by one, check clock.
TEST_F(SchedulerTest, ScheduleAllCause) {
	scheduler.schedule(eventFirst);
	scheduler.schedule(eventSecond);
	scheduler.schedule(eventThird);
	// Check chain size.
	EXPECT_EQ(3, scheduler.getChainSize());

	// Cause first event.
	Event causedEvent = scheduler.cause();
	// Is this really the first event?
	EXPECT_EQ(causedEvent, eventFirst);
	// Check chain size.
	EXPECT_EQ(2, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(1.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Cause second event.
	causedEvent = scheduler.cause();
	// Is this really the second event?
	EXPECT_EQ(causedEvent, eventSecond);
	// Check chain size.
	EXPECT_EQ(1, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(2.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Cause third event.
	causedEvent = scheduler.cause();
	// Is this really the third event?
	EXPECT_EQ(causedEvent, eventThird);
	// Check chain size.
	EXPECT_EQ(0, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(3.0, simulatorGlobals.getCurrentAbsoluteTime());
}

/// Schedules all events at once, out of order, causes one by one, check clock.
/// Results should be the same as in order above.
TEST_F(SchedulerTest, ScheduleAllOutOfOrderCause) {
	scheduler.schedule(eventThird);
	scheduler.schedule(eventFirst);
	scheduler.schedule(eventSecond);
	
	// Check chain size.
	EXPECT_EQ(3, scheduler.getChainSize());

	// Cause first event.
	Event causedEvent = scheduler.cause();
	// Is this really the first event?
	EXPECT_EQ(causedEvent, eventFirst);
	// Check chain size.
	EXPECT_EQ(2, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(1.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Cause second event.
	causedEvent = scheduler.cause();
	// Is this really the second event?
	EXPECT_EQ(causedEvent, eventSecond);
	// Check chain size.
	EXPECT_EQ(1, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(2.0, simulatorGlobals.getCurrentAbsoluteTime());

	// Cause third event.
	causedEvent = scheduler.cause();
	// Is this really the third event?
	EXPECT_EQ(causedEvent, eventThird);
	// Check chain size.
	EXPECT_EQ(0, scheduler.getChainSize());
	// Verify current simulator time.
	EXPECT_EQ(3.0, simulatorGlobals.getCurrentAbsoluteTime());
}

/// Tests events with same occurrence time.
TEST_F(SchedulerTest, ScheduleSameTime) {
	// Create events and associated entity objects.
	// All events have occur after time 0.0.
	std::shared_ptr<Message> message1(new Message("Front 1"));
	std::shared_ptr<Message> message2(new Message("Front 2"));
	std::shared_ptr<Message> message3(new Message("Front 3"));
	Event eventFront1(Event(0.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, message1));
	Event eventFront2(Event(0.0, EventType::BEGIN_SIMULATION, message2));
	Event eventFront3(Event(0.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, message3));

	// Schedule events.
	scheduler.schedule(eventFront1);
	scheduler.schedule(eventFront2);
	scheduler.schedule(eventFront3);

	// Now check the order in which the events were inserted at event chain.
	Event causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront1);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront2);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront3);
}

/// Tests events with same occurrence time.
TEST_F(SchedulerTest, ScheduleSameTimeAltOrder) {
	// Create events and associated entity objects.
	// All events have occur after time 0.0.
	std::shared_ptr<Message> message1(new Message("Front 1"));
	std::shared_ptr<Message> message2(new Message("Front 2"));
	std::shared_ptr<Message> message3(new Message("Front 3"));
	Event eventFront1(Event(0.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, message1));
	Event eventFront2(Event(0.0, EventType::BEGIN_SIMULATION, message2));
	Event eventFront3(Event(0.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, message3));

	// Schedule events.
	scheduler.schedule(eventFront3);
	scheduler.schedule(eventFront2);
	scheduler.schedule(eventFront1);

	// Now check the order in which the events were inserted at event chain.
	Event causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront3);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront2);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront1);
}

/// Tests events with same occurrence time, but using scheduleFront.
TEST_F(SchedulerTest, ScheduleFront) {
	// Create events and associated entity objects.
	// All events have occur after time 0.0.
	std::shared_ptr<Message> message1(new Message("Front 1"));
	std::shared_ptr<Message> message2(new Message("Front 2"));
	std::shared_ptr<Message> message3(new Message("Front 3"));
	Event eventFront1(Event(0.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, message1));
	Event eventFront2(Event(0.0, EventType::BEGIN_SIMULATION, message2));
	Event eventFront3(Event(0.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, message3));

	// Schedule events, but use scheduleFront to break order
	scheduler.schedule(eventFront3);
	scheduler.scheduleFront(eventFront2);
	scheduler.scheduleFront(eventFront1);
	// But an event at front, which could not occur before the others, explicitly breaking the event chain heap property
	scheduler.scheduleFront(eventThird);
	// Schedule this event normally, should be the last.
	scheduler.schedule(eventSecond);

	// Now check the order in which the events were inserted at event chain.
	Event causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventThird);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront1);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront2);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventFront3);
	causedEvent = scheduler.cause();
	EXPECT_EQ(causedEvent, eventSecond);
}

/// Tests removal of events from event chain.
TEST_F(SchedulerTest, RemoveEventFromChain) {
	// Create messages, tokens, events and associated entity objects.
	std::shared_ptr<Message> message1(new Message("dummy message 1"));
	std::shared_ptr<Message> message2(new Message("dummy message 2"));
	std::shared_ptr<Message> message3(new Message("dummy message 3"));
	std::shared_ptr<Token> token1(new Token(1, 0, message1, 0, 0));
	std::shared_ptr<Token> token2(new Token(2, 0, message2, 0, 0));
	std::shared_ptr<Token> token3(new Token(3, 0, message3, 0, 0));
	std::shared_ptr<Token> token4(new Token(4, 0, message3, 0, 0));

	// Schedule events. Notice token4 is not being used.
	Event event1(Event(0.0, EventType::BEGIN_SIMULATION, 0));
	Event event2(Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, token1));
	Event event3(Event(10.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, token1));
	Event event4(Event(2.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, token2));
	Event event5(Event(5.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, token2));
	Event event6(Event(8.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, token3));
	Event event7(Event(12.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, token3));
	Event event8(Event(30.0, EventType::END_SIMULATION, 0));
	
	// Schedule events. Order will be event 1, 2, 4, 5, 6, 3, 7, 8.
	scheduler.schedule(event1);
	scheduler.schedule(event2);
	scheduler.schedule(event3);
	scheduler.schedule(event4);
	scheduler.schedule(event5);
	scheduler.schedule(event6);
	scheduler.schedule(event7);
	scheduler.schedule(event8);
	EXPECT_EQ(8, scheduler.getChainSize());

	// Now remove events with corresponding tokens.
	// Remove events from token1. Two events.
	EXPECT_EQ(2, scheduler.removeEvents(token1));
	EXPECT_EQ(6, scheduler.getChainSize());

	// Remove events from token2.
	EXPECT_EQ(2, scheduler.removeEvents(token2));
	EXPECT_EQ(4, scheduler.getChainSize());
	
	// Remove events from token3.
	EXPECT_EQ(2, scheduler.removeEvents(token3));
	EXPECT_EQ(2, scheduler.getChainSize());

	// Remove events from token4. None.
	EXPECT_EQ(0, scheduler.removeEvents(token4));
	EXPECT_EQ(2, scheduler.getChainSize());
}