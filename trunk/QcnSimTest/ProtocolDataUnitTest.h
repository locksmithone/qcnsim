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

#pragma once

#include "../gtest/include/gtest/gtest.h"
#include "../QcnSim/ProtocolDataUnit.h"
#include "../QcnSim/Message.h"
#include "../QcnSim/Node.h"
#include <memory>

/// Fixture for PDU Tests.
class ProtocolDataUnitTest: public ::testing::Test {
protected:
	std::shared_ptr<Message> contents;
	SimulatorGlobals simulatorGlobals;

	/**
	 * Constructor.
	 *
	 * Do initializations here.
	 */
	ProtocolDataUnitTest();

	/// Destructor.
	//virtual ~TokenTest() {
	//}

	/// Set up common resources for tests here.
	//virtual void SetUp() {
	//}

	
	/// Perform clean-up here, if necessary.
	//virtual void TearDown() {
	//}
};