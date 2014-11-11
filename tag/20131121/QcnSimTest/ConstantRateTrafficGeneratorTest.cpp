/**
 * @author Marcos Portnoi
 * @date September 2013
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

#include "ConstantRateTrafficGeneratorTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
ConstantRateTrafficGeneratorTest::ConstantRateTrafficGeneratorTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "ConstantTrafficGeneratorTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
			simulatorGlobals.seedRandomNumberGenerator(1); // Use a fixed seed here for the tests.
}


/// Tests for constant rate generator.
TEST_F(ConstantRateTrafficGeneratorTest, ConstantRateRateGeneratorTest) {
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


