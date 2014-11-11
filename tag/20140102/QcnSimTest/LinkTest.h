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

// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#pragma once

#include "../gtest/include/gtest/gtest.h"
#include "../QcnSim/SimulatorGlobals.h"
#include "../QcnSim/Scheduler.h"
#include "../QcnSim/Node.h"
#include "../QcnSim/Link.h"
#include "../QcnSim/ProtocolDataUnit.h"
#include "../QcnSim/EventType.h"
#include "../QcnSim/LinkReturnType.h"
#include <string>

/// Fixture for Facility Tests.
class LinkTest: public ::testing::Test {
protected:
	SimulatorGlobals simulatorGlobals;
	Scheduler scheduler;
		
	/**
	 * Constructor.
	 *
	 * Do initializations here.
	 */
	LinkTest();

	
};