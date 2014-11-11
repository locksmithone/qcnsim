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

#include "WeibullTrafficGeneratorTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
WeibullTrafficGeneratorTest::WeibullTrafficGeneratorTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "WeibullTrafficGeneratorTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
			simulatorGlobals.seedRandomNumberGenerator(1); // Use a fixed seed here for the tests.
}

/// Graphical validations for Weibull variate generator.
/// @note Important! For this to work and files to be generated, this test must be run *outside* of VS 2013 Gtest plugin. This plugin redirects outputs.
TEST_F(WeibullTrafficGeneratorTest, WeibullVariateGeneratorTest) {
	std::shared_ptr<Message> source(new Message("This is a dummy entity for source."));
	std::shared_ptr<Message> destination(new Message("This is a dummy entity for destination."));
	std::shared_ptr<Message> tokenContents(new Message("This is a dummy Token contents."));
	unsigned int seed = 1;
	std::shared_ptr<Token> token(nullptr);
	std::ofstream outputFile; // Output file handle.
	int numberOfSamples = 100000; // Number of samples to generate for variate.
	std::map<std::string, WeibullTrafficGenerator> weibullGeneratorMap; // Map to control weibull generator instances and their output filenames. (String comes first because the key must be constant.)

	// Set the fixed seed.
	simulatorGlobals.seedRandomNumberGenerator(seed);
	
	// Create weibull generators and populate maps with their references and output files.
	weibullGeneratorMap.insert(std::pair<std::string, WeibullTrafficGenerator>("weibull_Scale1.0_Shape5.0.csv",
		WeibullTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, 1, 5)));
	weibullGeneratorMap.insert(std::pair<std::string, WeibullTrafficGenerator>("weibull_Scale1.0_Shape1.0.csv",
		WeibullTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, 1, 1)));
	weibullGeneratorMap.insert(std::pair<std::string, WeibullTrafficGenerator>("weibull_Scale1.0_Shape2.0.csv", 
		WeibullTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, 1, 2)));
	weibullGeneratorMap.insert(std::pair<std::string, WeibullTrafficGenerator>("weibull_Scale1.0_Shape0.5.csv", 
		WeibullTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, tokenContents, source, destination, 1, 1, 0.5)));
	
	// Now generate all weibull samples for each generator.
	for (auto &weibullGeneratorMapIterator : weibullGeneratorMap) {
		// Turn on generator.
		weibullGeneratorMapIterator.second.turnOn();
		// Open file for output.
		outputFile.open(weibullGeneratorMapIterator.first);
		// Generate samples.
		for (int i = 0; i < numberOfSamples; ++i) {
			weibullGeneratorMapIterator.second.createInstanceTrafficEvent();
			outputFile << scheduler.cause().occurAfterTime << std::endl;
		}
		outputFile.close();
	}
}
