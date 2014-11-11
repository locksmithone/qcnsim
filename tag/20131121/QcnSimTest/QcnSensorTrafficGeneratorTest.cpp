/**
 * @author Marcos Portnoi
 * @date November 2013
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

#include "QcnSensorTrafficGeneratorTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
QcnSensorTrafficGeneratorTest::QcnSensorTrafficGeneratorTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "QcnSensorTrafficGeneratorTest")),
scheduler(Scheduler(simulatorGlobals)) {
}

/// Test Constructor with tokenizer, data from file, instantiate objects as file is read.
TEST_F(QcnSensorTrafficGeneratorTest, FileReaderObjectInstantiation) {
	std::map<unsigned int, std::shared_ptr<QcnSensorTrafficGenerator>> qcnSensorGeneratorMap;
	std::map<unsigned int, std::shared_ptr<SeismicEventData>> seismicEventDataMap;
	std::shared_ptr<SeismicEventData> seismicEventData(nullptr); // Will hold all seismic event objects instantiated from file.
	std::ifstream inputFile; // Input file handle.
	std::ofstream outputFile; // Output file handle
	std::string line; // Line to be read from input file.
	//int lineCount = 0;

	// Open file for input.
	inputFile.open("seismicEventData.csv");
	// Now read all file. Discard line if it begins with string "ID", meaning this is the header.
	while (std::getline(inputFile, line)) {
		if (line.substr(0, 2) != "ID") {
			// It is not the header. Create seismic event object.
			seismicEventData = std::make_shared<SeismicEventData>(SeismicEventData(line, ','));
			// If QCN sensor with same sensor id does not yet exist, instantiate one and put into map.
			if (qcnSensorGeneratorMap.find(seismicEventData->qcnExplorerSensorId) == qcnSensorGeneratorMap.end()) {
				qcnSensorGeneratorMap.insert(std::pair<unsigned int, std::shared_ptr<QcnSensorTrafficGenerator>>(seismicEventData->qcnExplorerSensorId,
					std::shared_ptr<QcnSensorTrafficGenerator>(new QcnSensorTrafficGenerator(simulatorGlobals, scheduler, EventType::TRAFFIC_GENERATOR_ARRIVAL, seismicEventData,
					nullptr, nullptr, 1, seismicEventData->latitude, seismicEventData->longitude, seismicEventData->qcnExplorerSensorId, seismicEventData->regionId))));
			}
			seismicEventDataMap.insert(std::pair<unsigned int, std::shared_ptr<SeismicEventData>>(seismicEventData->qcnExplorerSensorId, seismicEventData));
			//++lineCount;
		}
	}
	inputFile.close();
	// Now iterate through seismic event data and compare with related QCN sensor generators.
	for (auto seismicEventDataMapIterator : seismicEventDataMap) {
		EXPECT_EQ(seismicEventDataMapIterator.second->latitude, qcnSensorGeneratorMap.at(seismicEventDataMapIterator.first)->getLatitude());
		EXPECT_EQ(seismicEventDataMapIterator.second->longitude, qcnSensorGeneratorMap.at(seismicEventDataMapIterator.first)->getLongitude());
		EXPECT_EQ(seismicEventDataMapIterator.second->regionId, qcnSensorGeneratorMap.at(seismicEventDataMapIterator.first)->getRegionId());
		EXPECT_EQ(seismicEventDataMapIterator.second->latitude, qcnSensorGeneratorMap.at(seismicEventDataMapIterator.first)->getLatitude());
	}

	// Now output the objects' info to a file for comparison with original.
	outputFile.open("qcnSensorGenerators.csv");
	for (auto qcnSensorGeneratorMapIterator : qcnSensorGeneratorMap) {
		outputFile << std::setprecision(10) << qcnSensorGeneratorMapIterator.second->getQcnExplorerSensorId() << ":";
		outputFile << qcnSensorGeneratorMapIterator.second->getLatitude() << ":";
		outputFile << qcnSensorGeneratorMapIterator.second->getLongitude() << ":";
		outputFile << qcnSensorGeneratorMapIterator.second->getRegionId() << ":";
		outputFile << qcnSensorGeneratorMapIterator.second->getTokenContents() << std::endl;
	}
	outputFile.close();
}
	