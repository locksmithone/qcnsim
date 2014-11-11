/**
 * @author Marcos Portnoi
 * @date July 2013
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

#include "FacilityTest.h"

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
FacilityTest::FacilityTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "FacilityTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
}

/// Construct one facility, one server, and get several attributes.
TEST_F(FacilityTest, ConstructorOne) {
	Facility facility1 = Facility("Facility One", simulatorGlobals, scheduler);
	EXPECT_EQ("Facility One", facility1.getName());
	EXPECT_EQ(0, facility1.getDequeuedTokensCount());
	EXPECT_EQ(0, facility1.getDroppedTokensCount());
	EXPECT_EQ(0, facility1.getLastQueueChangeTime());
	EXPECT_EQ(0, facility1.getMaxQueueSize());
	EXPECT_EQ(0, facility1.getMeanBusyPeriod());
	EXPECT_EQ(0, facility1.getMeanQueueLength());
	EXPECT_EQ(1, facility1.getNumberOfServers());
	EXPECT_EQ(0, facility1.getPreemptedTokensCount());
	EXPECT_EQ(0, facility1.getQueueSize());
	EXPECT_EQ(0, facility1.getReleasedTokensCount());
	EXPECT_EQ(0, facility1.getSumBusyTime());
	EXPECT_EQ(0, facility1.getSumLengthTimeProduct());
	EXPECT_EQ(0, facility1.getUtilization());
	EXPECT_EQ(0, facility1.getDequeuedTokensCount());
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
}

/// Construct many facilities, perform select getters.
TEST_F(FacilityTest, ConstructorMany) {
	Facility facility1 = Facility("Facility One", simulatorGlobals, scheduler);
	Facility facility2 = Facility("Facility Two", simulatorGlobals, scheduler);
	Facility facility3 = Facility("Facility Three", simulatorGlobals, scheduler);
	Facility facility4 = Facility("Facility Four", simulatorGlobals, scheduler);
	EXPECT_EQ("Facility One", facility1.getName());
	EXPECT_EQ("Facility Two", facility2.getName());
	EXPECT_EQ("Facility Three", facility3.getName());
	EXPECT_EQ("Facility Four", facility4.getName());
	// Perform various tests on select facilities.
	EXPECT_EQ(0, facility1.getDequeuedTokensCount());
	EXPECT_EQ(0, facility2.getDroppedTokensCount());
	EXPECT_EQ(0, facility3.getLastQueueChangeTime());
	EXPECT_EQ(0, facility4.getMaxQueueSize());
	EXPECT_EQ(0, facility2.getMeanBusyPeriod());
	EXPECT_EQ(0, facility3.getMeanQueueLength());
	EXPECT_EQ(1, facility1.getNumberOfServers());
	EXPECT_EQ(1, facility2.getNumberOfServers());
	EXPECT_EQ(1, facility3.getNumberOfServers());
	EXPECT_EQ(1, facility4.getNumberOfServers());
	EXPECT_EQ(0, facility2.getPreemptedTokensCount());
	EXPECT_EQ(0, facility3.getQueueSize());
	EXPECT_EQ(0, facility4.getReleasedTokensCount());
	EXPECT_EQ(0, facility1.getSumBusyTime());
	EXPECT_EQ(0, facility2.getSumLengthTimeProduct());
	EXPECT_EQ(0, facility3.getUtilization());
	EXPECT_EQ(0, facility4.getDequeuedTokensCount());
	EXPECT_FALSE(facility3.isBusy());
	EXPECT_TRUE(facility2.isUp());
}

/// Construct facilities with many servers.
TEST_F(FacilityTest, ConstructorManyServers) {
	Facility facility1 = Facility("Facility One", 10, simulatorGlobals, scheduler);
	Facility facility2 = Facility("Facility Two", 20, simulatorGlobals, scheduler);
	Facility facility3 = Facility("Facility Three", 30, simulatorGlobals, scheduler);
	Facility facility4 = Facility("Facility Four", 40, simulatorGlobals, scheduler);
	EXPECT_EQ("Facility One", facility1.getName());
	EXPECT_EQ("Facility Two", facility2.getName());
	EXPECT_EQ("Facility Three", facility3.getName());
	EXPECT_EQ("Facility Four", facility4.getName());
	// Perform various tests on select facilities.
	EXPECT_EQ(0, facility1.getDequeuedTokensCount());
	EXPECT_EQ(0, facility2.getDroppedTokensCount());
	EXPECT_EQ(0, facility3.getLastQueueChangeTime());
	EXPECT_EQ(0, facility4.getMaxQueueSize());
	EXPECT_EQ(0, facility2.getMeanBusyPeriod());
	EXPECT_EQ(0, facility3.getMeanQueueLength());
	EXPECT_EQ(10, facility1.getNumberOfServers());
	EXPECT_EQ(20, facility2.getNumberOfServers());
	EXPECT_EQ(30, facility3.getNumberOfServers());
	EXPECT_EQ(40, facility4.getNumberOfServers());
	EXPECT_EQ(0, facility2.getPreemptedTokensCount());
	EXPECT_EQ(0, facility3.getQueueSize());
	EXPECT_EQ(0, facility4.getReleasedTokensCount());
	EXPECT_EQ(0, facility1.getSumBusyTime());
	EXPECT_EQ(0, facility2.getSumLengthTimeProduct());
	EXPECT_EQ(0, facility3.getUtilization());
	EXPECT_EQ(0, facility4.getDequeuedTokensCount());
	EXPECT_FALSE(facility3.isBusy());
	EXPECT_TRUE(facility2.isUp());
}

TEST_F(FacilityTest, Preempt) {
	// Function not yet implemented.
}

TEST_F(FacilityTest, PurgeQueue) {
	// Build a facility with one server.
	Facility facility1 = Facility("Facility One", 1, simulatorGlobals, scheduler);
	// Build some tokens.
	std::shared_ptr<Message> dummyEntitySource(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> dummyEntityDestination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> dummyEntity(new Message("This is a dummy Token contents."));
	std::shared_ptr<Token> tokenService(new Token(42, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token1(new Token(1, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token2(new Token(2, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token3(new Token(3, 3, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token4(new Token(4, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token5(new Token(5, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token6(new Token(6, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token7(new Token(7, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token8(new Token(8, 3, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token9(new Token(9, 4, dummyEntity, dummyEntitySource, dummyEntityDestination));
		
	// Free facility.
	// Facility must be free and up.
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Request service for tokens; first will go into service, others will be enqueued in order of priority.
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(tokenService, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token2, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token3, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token4, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token5, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token6, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token7, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token8, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token9, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(9, facility1.getQueueSize());
	EXPECT_EQ(9, facility1.getMaxQueueSize());
	// Purge queue by making the facility down.
	// First advance clock.
	simulatorGlobals.setCurrentAbsoluteTime(10.0);
	EXPECT_EQ(9, facility1.setDown());
	EXPECT_EQ(0, facility1.getDequeuedTokensCount()); // Tokens were dropped, not dequeued.
	EXPECT_EQ(9, facility1.getDroppedTokensCount());
}

/// Attempts several instances of requests/releases for a facility (free, busy, down) and test outcomes.
TEST_F(FacilityTest, RequestReleaseQueue) {
	// Build a facility with one server.
	Facility facility1 = Facility("Facility One", 1, simulatorGlobals, scheduler);
	// Build some tokens.
	std::shared_ptr<Message> dummyEntitySource(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> dummyEntityDestination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> dummyEntity(new Message("This is a dummy Token contents."));
	std::shared_ptr<Token> token1(new Token(1, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token2(new Token(2, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token3(new Token(3, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	// Build some events.
	//Event event1 = Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, dummyEntity);
		
	// Free facility.
	// Facility must be free and up.
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Request service for one token; it must be put into service.
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	// Queue must be empty.
	EXPECT_EQ(0, facility1.getQueueSize());
	// And facility must be up and busy.
	EXPECT_TRUE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());

	// Force the clock to advance such that statistics can be calculated. (This is normally done by the scheduler.)
	simulatorGlobals.setCurrentAbsoluteTime(2.0);
	//First, let's try to release a token not in service.
	EXPECT_EQ(FacilityReturnType::TOKEN_NOT_FOUND, facility1.release(token2));
	// Now let's release the correct token and test outcomes.
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_QUEUE_EMPTY, facility1.release(token1));
	// Facility must be up and free.
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Nothing in queue.
	EXPECT_EQ(0, facility1.getQueueSize());
	// And check a few statistics.
	EXPECT_EQ(0, facility1.getDequeuedTokensCount()); // Nothing was dequeued!
	EXPECT_EQ(0, facility1.getDroppedTokensCount());
	EXPECT_EQ(0.0, facility1.getLastQueueChangeTime());
	EXPECT_EQ(2.0, facility1.getMeanBusyPeriod());
	EXPECT_EQ(1, facility1.getUtilization());
	EXPECT_EQ(1, facility1.getReleasedTokensCount()); // And there was one release.

	// Busy facility; token will be enqueued in this part.
	// Let's force the clock to advance.
	simulatorGlobals.setCurrentAbsoluteTime(4.0);
	// Utilization now will not be 1, but 0.5 (busy time (2.0) / total time (4.0))
	EXPECT_EQ(0.5, facility1.getUtilization());
	// Put one token into service.
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	// Facility must be up and busy.
	EXPECT_TRUE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Request service for another token before releasing the first. It will be enqueued.
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token2, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	// Check the queue size and busy status.
	EXPECT_EQ(1, facility1.getQueueSize());
	EXPECT_TRUE(facility1.isBusy());

	// Now let's end service, release token, and test dequeuing functionality.
	// Advance clock.
	simulatorGlobals.setCurrentAbsoluteTime(6.0);
	// Get utilization. Notice that, even though the facility is still busy at clock 6.0, meaning that
	// the current total busy time is 2.0 (previous token) + 2.0 (service for the current token started at 4.0, now it is 6.0),
	// the sumBusy statistics is only updated *after* the token in service is released, by calling release function.
	EXPECT_EQ(2.0/6.0, facility1.getUtilization()); // Current is 4/6, but simBusy was *not* updated yet!
	//First, let's try to release tokens not in service.
	EXPECT_EQ(FacilityReturnType::TOKEN_NOT_FOUND, facility1.release(token3));
	EXPECT_EQ(FacilityReturnType::TOKEN_NOT_FOUND, facility1.release(token2)); // Remember, token2 is in the queue, not in service.
	// Now let's release the correct token and test outcomes.
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token1));
	// Facility must be up and free, since service event for dequeued token was *not* caused yet.
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Nothing in queue now.
	EXPECT_EQ(0, facility1.getQueueSize());
	// And check a few statistics.
	EXPECT_EQ(1, facility1.getDequeuedTokensCount());
	EXPECT_EQ(0, facility1.getDroppedTokensCount());
	EXPECT_EQ(6.0, facility1.getLastQueueChangeTime());
	EXPECT_EQ(4.0/2, facility1.getMeanBusyPeriod());
	EXPECT_EQ(4.0/6.0, facility1.getUtilization()); // Now busyTime was updated, utilization will reflect the most current state.
	EXPECT_EQ(2, facility1.getReleasedTokensCount()); // And there was one release.

	// Dequeuing done. Now let's "cause" the service request event and put the dequeued token into service, and queue two tokens.
	Event event = scheduler.cause();
	// Check this event and current clock.
	EXPECT_EQ(6.0, simulatorGlobals.getCurrentAbsoluteTime());
	EXPECT_EQ(Event(0.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, token2), event);
	// Put this dequeued token into service.
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token2, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	// Check the queue size and busy status.
	EXPECT_EQ(0, facility1.getQueueSize());
	EXPECT_TRUE(facility1.isBusy());
	// Enqueue two more tokens, by requesting service while the facility is busy.
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token3, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	// Check the queue size and busy status.
	EXPECT_EQ(2, facility1.getQueueSize());
	EXPECT_TRUE(facility1.isBusy());
	// Check the order in which the tokens were enqueued (all tokens here have the same priority).
	// Advance clock, release, cause, etc.
	simulatorGlobals.setCurrentAbsoluteTime(10.0);
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token2));
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_EQ((2.0 + 2.0 + 4.0)/10.0, facility1.getUtilization());
	EXPECT_EQ(2, facility1.getDequeuedTokensCount());
	EXPECT_EQ(3, facility1.getReleasedTokensCount());
	EXPECT_EQ(1, facility1.getQueueSize());
	EXPECT_EQ(0, facility1.getDroppedTokensCount());
	EXPECT_EQ(10.0, facility1.getLastQueueChangeTime());
	EXPECT_EQ(8.0/3, facility1.getMeanBusyPeriod());
	event = scheduler.cause();
	// Token at head should be token1, second is token3
	EXPECT_EQ(token1, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_TRUE(facility1.isBusy());
	// Advance clock, request service, release, etc.
	simulatorGlobals.setCurrentAbsoluteTime(11.0);
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token1));
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_EQ((2.0 + 2.0 + 4.0 + 1.0)/11.0, facility1.getUtilization());
	EXPECT_EQ(3, facility1.getDequeuedTokensCount());
	EXPECT_EQ(4, facility1.getReleasedTokensCount());
	EXPECT_EQ(0, facility1.getQueueSize());
	EXPECT_EQ(0, facility1.getDroppedTokensCount());
	EXPECT_EQ(2, facility1.getMaxQueueSize());
	EXPECT_EQ(11.0, facility1.getLastQueueChangeTime());
	EXPECT_EQ(9.0/4, facility1.getMeanBusyPeriod());
	event = scheduler.cause();
	// Now we have token3.
	EXPECT_EQ(token3, event.entity);	
}

/// Tests queueing for different priorities, service request.
TEST_F(FacilityTest, QueuePriorityRequest) {
	// Build a facility with one server.
	Facility facility1 = Facility("Facility One", 1, simulatorGlobals, scheduler);
	// Build some tokens.
	std::shared_ptr<Message> dummyEntitySource(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> dummyEntityDestination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> dummyEntity(new Message("This is a dummy Token contents."));
	std::shared_ptr<Token> tokenService(new Token(42, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token1(new Token(1, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token2(new Token(2, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token3(new Token(3, 3, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token4(new Token(4, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token5(new Token(5, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token6(new Token(6, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token7(new Token(7, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token8(new Token(8, 3, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token9(new Token(9, 4, dummyEntity, dummyEntitySource, dummyEntityDestination));
		
	// Free facility.
	// Facility must be free and up.
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Request service for tokens; first will go into service, others will be enqueued in order of priority.
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(tokenService, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token2, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token3, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token4, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token5, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token6, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token7, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token8, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token9, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(9, facility1.getQueueSize());
	EXPECT_EQ(9, facility1.getMaxQueueSize());
	// Test the order. It should be tokens 9,3,8,2,6,7,1,4,5
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(tokenService));
	Event event = scheduler.cause();
	EXPECT_EQ(token9, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token9, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token9));
	event = scheduler.cause();
	EXPECT_EQ(token3, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token3, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token3));
	event = scheduler.cause();
	EXPECT_EQ(token8, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token8, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token8));
	event = scheduler.cause();
	EXPECT_EQ(token2, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token2, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token2));
	event = scheduler.cause();
	EXPECT_EQ(token6, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token6, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token6));
	event = scheduler.cause();
	EXPECT_EQ(token7, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token7, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token7));
	event = scheduler.cause();
	EXPECT_EQ(token1, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token1));
	event = scheduler.cause();
	EXPECT_EQ(token4, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token4, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token4));
	event = scheduler.cause();
	EXPECT_EQ(token5, event.entity);
	
}


/// Tests a facility with more than one server.
TEST_F(FacilityTest, RequestMultipleServers) {
	// Build a facility with 3 servers.
	Facility facility1 = Facility("Facility One", 3, simulatorGlobals, scheduler);
	EXPECT_EQ(3, facility1.getNumberOfServers());
	// Build some tokens.
	std::shared_ptr<Message> dummyEntitySource(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> dummyEntityDestination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> dummyEntity(new Message("This is a dummy Token contents."));
	std::shared_ptr<Token> tokenService(new Token(42, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token1(new Token(1, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token2(new Token(2, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token3(new Token(3, 3, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token4(new Token(4, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token5(new Token(5, 1, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token6(new Token(6, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token7(new Token(7, 2, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token8(new Token(8, 3, dummyEntity, dummyEntitySource, dummyEntityDestination));
	std::shared_ptr<Token> token9(new Token(9, 4, dummyEntity, dummyEntitySource, dummyEntityDestination));
		
	// Free facility.
	// Facility must be free and up.
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_TRUE(facility1.isUp());
	// Request service for tokens; first 3 will go into service, others will be enqueued in order of priority.
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(tokenService, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token1, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_FALSE(facility1.isBusy());
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token2, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_TRUE(facility1.isBusy());
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token3, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token4, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token5, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token6, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token7, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token8, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_ENQUEUED, facility1.request(token9, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(7, facility1.getQueueSize());
	EXPECT_EQ(7, facility1.getMaxQueueSize());
	// Let's release and request service for tokens in queue.
	// Queue order should be tokens 9,3,8,6,7,4,5
	EXPECT_EQ(FacilityReturnType::TOKEN_NOT_FOUND, facility1.release(token3)); // This one is not in service.
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token2));
	EXPECT_FALSE(facility1.isBusy());
	Event event = scheduler.cause();
	EXPECT_EQ(token9, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token9, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(tokenService));
	event = scheduler.cause();
	EXPECT_EQ(token3, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token3, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token1));
	event = scheduler.cause();
	EXPECT_EQ(token8, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token8, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token9));
	event = scheduler.cause();
	EXPECT_EQ(token6, event.entity);
	EXPECT_EQ(FacilityReturnType::TOKEN_PUT_IN_SERVICE, facility1.request(token6, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY));
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token8));
	event = scheduler.cause();
	EXPECT_EQ(token7, event.entity);
	// Let's release more tokens one before putting the just dequeued into service. Notice that, in normal operation, this should never happen, since the
	// next event in the chain is forcefully the service request for the just released token; another release wouldn't occur before.
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token3)); // Now there are two free servers within the facility.
	EXPECT_EQ(FacilityReturnType::TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED, facility1.release(token6)); // Now all servers are free.
	event = scheduler.cause();
	EXPECT_EQ(token5, event.entity); // Token 5 was released last, so its event took the head of the event chain.
	event = scheduler.cause();
	EXPECT_EQ(token4, event.entity); // Now we have token 4.
	// And there should be an empty queue.
	EXPECT_EQ(0, facility1.getQueueSize());
}