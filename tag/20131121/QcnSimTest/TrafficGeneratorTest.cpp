/**
 * @author Marcos Portnoi
 * @date August 2013
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

#include "TrafficGeneratorTest.h"

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
TrafficGeneratorTest::TrafficGeneratorTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "TrafficGeneratorTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
			simulatorGlobals.seedRandomNumberGenerator(1); // Use a fixed seed here for the tests.
}


/// Functionality of random number generator from SimulatorGlobals.
TEST_F(TrafficGeneratorTest, RandomGeneratorTest) {
	SimulatorGlobals simulatorGlobalsOther = SimulatorGlobals();
	// This test below will always fail... or almost always, since the default seed is random (from random_device).
	//EXPECT_EQ(3435973836, simulatorGlobalsOther.getRandomNumberGeneratorSeed());
	// Now let's test with the default, determined seed.
	EXPECT_EQ(1, simulatorGlobals.getRandomNumberGeneratorSeed());

}

/// Tests for Exponential variate generator.
TEST_F(TrafficGeneratorTest, ExponentialVariateGeneratorTest) {
	std::shared_ptr<Message> source(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> destination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> tokenContents(new Message("This is a dummy Token contents."));
	unsigned int seed = 1;
	double tau = 1.0;
	std::shared_ptr<Token> token(nullptr);

	// Set the fixed seed.
	simulatorGlobals.seedRandomNumberGenerator(seed);
	ExponentialTrafficGenerator exponentialGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, tau);
	// Keep generator off, attempt to generate one instance of traffic. Member token is null at this point.
	EXPECT_EQ(token, exponentialGenerator.createInstanceTrafficEvent());
	// Now turn generator On, generate one instance of traffic. Test outcomes.
	exponentialGenerator.turnOn();
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(1, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	// Get the event that was scheduled.
	Event event = scheduler.cause();
	// Check the event time and see whether it is always the same value per the seed.
	EXPECT_DOUBLE_EQ(0.539605826511863, event.occurAfterTime);
	
	// Create another exponential generator and see whether it begins with a different random sequence.
	// It should; however, these tests prove that all exponential generators follow *one* unique pseudorandom sequence, determined by
	// simulatorGlobals random engine and its seed. That is the desired behavior.
	ExponentialTrafficGenerator exponentialGeneratorSecond(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, tau);
	exponentialGeneratorSecond.turnOn();
	token = exponentialGeneratorSecond.createInstanceTrafficEvent();
	EXPECT_EQ(2, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Check the event time and see whether it always the same value per the seed, but different than another instance of the expo. generator.
	EXPECT_DOUBLE_EQ(5.8727248609813936, event.occurAfterTime); // 5.82 for a tau of 1, isn't it too large?

	// Get two more values, and then change the seed to another value and test two generations. The new values after seeding should be different than the next two here.
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(3, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Check the event time and see whether it always the same value per the seed, but different than another instance of the expo. generator.
	EXPECT_DOUBLE_EQ(1.2741252381599264, event.occurAfterTime);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(4, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Check the event time and see whether it always the same value per the seed, but different than another instance of the expo. generator.
	EXPECT_DOUBLE_EQ(2.6964778379468424, event.occurAfterTime);
	// Change global seed, generate two tokens, compare with values generated previously, in another test, for seed = 2.
	simulatorGlobals.seedRandomNumberGenerator(2);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(5, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Check the event time and see whether it always the same value per the seed, but different than another instance of the expo. generator.
	EXPECT_DOUBLE_EQ(0.57269198962718915, event.occurAfterTime);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(6, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Check the event time and see whether it always the same value per the seed, but different than another instance of the expo. generator.
	EXPECT_DOUBLE_EQ(0.20466788427640734, event.occurAfterTime);
	// Generate one more, just to get the third value on the sequence of seed 2.
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(7, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Check the event time and see whether it always the same value per the seed, but different than another instance of the expo. generator.
	EXPECT_DOUBLE_EQ(0.026268236846611673, event.occurAfterTime);
	
	// Seed again, generate two tokens, compare with previous values. Does reseeding "reset" the same sequence?
	simulatorGlobals.seedRandomNumberGenerator(2);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(8, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Event time should be the same as first prior event after seeding.
	EXPECT_DOUBLE_EQ(0.57269198962718915, event.occurAfterTime);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(9, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Event time should be the same as second prior event after seeding.
	EXPECT_DOUBLE_EQ(0.20466788427640734, event.occurAfterTime);

	// Now let's reseed with 2, generate one token with one generator, another with second generator, another with first generator.
	// The first value from the first generator should match the previous value just after seeding, but the subsequent values should be different, if the random number generator is really globally unique.
	simulatorGlobals.seedRandomNumberGenerator(2);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(10, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Event time should be the same as first prior event after seeding.
	EXPECT_DOUBLE_EQ(0.57269198962718915, event.occurAfterTime);
	token = exponentialGeneratorSecond.createInstanceTrafficEvent();
	EXPECT_EQ(11, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Event time *should* be the same as second prior event after seeding.
	EXPECT_DOUBLE_EQ(0.20466788427640734, event.occurAfterTime);
	token = exponentialGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(12, token->id);
	// Get the event that was scheduled.
	event = scheduler.cause();
	// Event time *should* be the same as third event from seed 2. That proves that the same pseudorandom sequence is unique per simulation.
	EXPECT_DOUBLE_EQ(0.026268236846611673, event.occurAfterTime);

	// Now test generator's token counters.
	EXPECT_EQ(10, exponentialGenerator.getTokensGeneratedCount());
	EXPECT_EQ(2, exponentialGeneratorSecond.getTokensGeneratedCount());

}

/// Tests for constant rate generator.
TEST_F(TrafficGeneratorTest, ConstantRateGeneratorTest) {
	std::shared_ptr<Message> source(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> destination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> tokenContents(new Message("This is a dummy Token contents."));
	double interval = 3.0;
	std::shared_ptr<Token> token(nullptr);

	// Construct CBR generator.
	ConstantRateTrafficGenerator cbrGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, interval);
	// Keep generator off, attempt to generate one instance of traffic. Member token is null at this point, since cbr generator is Off.
	EXPECT_EQ(token, cbrGenerator.createInstanceTrafficEvent());
	// Now turn generator On, generate one instance of traffic. Test outcomes.
	cbrGenerator.turnOn();
	token = cbrGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(1, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	// Get the event that was scheduled.
	Event event = scheduler.cause();
	// Check the event time and see whether it is as expected.
	EXPECT_DOUBLE_EQ(3.0, event.occurAfterTime);
	
	// Generate a few other events, test the current clock time.
	// Generate two arrivals in a row; must occur at the same time.
	token = cbrGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(2, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	token = cbrGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(3, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	// Get the events, test the simulator clock.
	event = scheduler.cause();
	EXPECT_DOUBLE_EQ(6.0, simulatorGlobals.getCurrentAbsoluteTime());
	// Get the events, test the occurrence times.
	event = scheduler.cause();
	EXPECT_DOUBLE_EQ(6.0, simulatorGlobals.getCurrentAbsoluteTime());
	// Now generate events in turns to test sequence of occurrence times.
	token = cbrGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(4, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	event = scheduler.cause();
	EXPECT_DOUBLE_EQ(9.0, simulatorGlobals.getCurrentAbsoluteTime());
	token = cbrGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(5, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	event = scheduler.cause();
	EXPECT_DOUBLE_EQ(12.0, simulatorGlobals.getCurrentAbsoluteTime());
	token = cbrGenerator.createInstanceTrafficEvent();
	EXPECT_EQ(6, token->id);
	EXPECT_EQ(1, token->priority);
	EXPECT_EQ(tokenContents, token->associatedEntity);
	EXPECT_EQ(source, token->source);
	EXPECT_EQ(destination, token->destination);
	event = scheduler.cause();
	EXPECT_DOUBLE_EQ(15.0, simulatorGlobals.getCurrentAbsoluteTime());
}


