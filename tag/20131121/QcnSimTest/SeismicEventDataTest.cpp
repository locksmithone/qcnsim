/**
 * @author Marcos Portnoi
 * @date October 2013
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

#include "SeismicEventDataTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
SeismicEventDataTest::SeismicEventDataTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "SeismicEventDataTest")),
scheduler(Scheduler(simulatorGlobals)) {
}

/// Test Constructor with tokenizer, data from file.
TEST_F(SeismicEventDataTest, ConstructorTokenizer) {
	std::vector<SeismicEventData> seismicEventData; // Will hold all seismic event objects instantiated from file.
	std::ifstream inputFile; // Input file handle.
	std::ofstream outputFile; // Output file handle
	std::string line; // Line to be read from input file.
	int lineCount = 0;

	// Open file for input.
	inputFile.open("seismicEventData.csv");
	// Now read all file. Discard line if it begins with string "ID", meaning this is the header.
	while (std::getline(inputFile, line)) {
		if (line.substr(0, 2) != "ID") {
			// It is not the header. Create object, put into vector.
			seismicEventData.push_back(SeismicEventData(line, ','));
			++lineCount;
		}
	}
	inputFile.close();
	// Were all lines instantiated into objects?
	EXPECT_EQ(lineCount, seismicEventData.size());

	// Now output the objects to a file for comparison with original.
	outputFile.open("seismicObjects.csv");
	for (auto seismicEventDataIterator : seismicEventData) {
		outputFile << std::setprecision(10) << seismicEventDataIterator.qcnExplorerSensorId << ":";
		outputFile << seismicEventDataIterator.latitude << ":";
		outputFile << seismicEventDataIterator.longitude << ":";
		outputFile << seismicEventDataIterator.magnitude << ":";
		outputFile << seismicEventDataIterator.eventTime << ":";
		outputFile << seismicEventDataIterator.distance << ":";
		outputFile << seismicEventDataIterator.regionId << std::endl;
	}
	outputFile.close();
}
	