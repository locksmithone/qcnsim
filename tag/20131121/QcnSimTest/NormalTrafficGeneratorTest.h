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

#pragma once

#include "../gtest/include/gtest/gtest.h"
#include "../QcnSim/SimulatorGlobals.h"
#include "../QcnSim/TrafficGenerator.h"
#include "../QcnSim/NormalTrafficGenerator.h"
#include "../QcnSim/EventType.h"
#include "../QcnSim/Event.h"
#include "../QcnSim/Message.h"
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>

/// Fixture for NormalTrafficGenerator classes Tests.
class NormalTrafficGeneratorTest: public ::testing::Test {
protected:
	SimulatorGlobals simulatorGlobals;
	Scheduler scheduler;
		
	/**
	 * Constructor.
	 *
	 * Do initializations here.
	 */
	NormalTrafficGeneratorTest();

	/// Destructor.
	//virtual ~TokenTest() {
	//}

	/// Set up common resources for tests here.
	/*virtual void SetUp() {
		eventFirst = Event(1.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0);
		eventSecond = Event(2.0, EventType::RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY, 0);
		eventThird = Event(3.0, EventType::REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY, 0);
	}*/

	
	/// Perform clean-up here, if necessary.
	//virtual void TearDown() {
	//}
};