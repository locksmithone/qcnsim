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

#pragma once

#include "../gtest/include/gtest/gtest.h"
#include "../QcnSim/SimulatorGlobals.h"
#include "../QcnSim/Scheduler.h"
#include "../QcnSim/SeismicEventData.h"
#include <iostream>
#include <fstream>
#include <vector>

/// Fixture for Seismic Event Tests.
class SeismicEventDataTest: public ::testing::Test {
protected:
	SimulatorGlobals simulatorGlobals;
	Scheduler scheduler;

	/**
	 * Constructor.
	 *
	 * Do initializations here.
	 */
	SeismicEventDataTest();


};

// File format:
//ID,lat,lng,mag,obsvTime,hypoCentDist,regionID
//0,32.79095,-117.0624,0.73041597342086,13.35841033437,46.662854098045,1
//0,32.79095,-117.0624,0.14608319468417,11.939035989821,46.662854098045,1
//1,32.79726,-117.06936,0.68341775176201,13.029891471409,46.592415422896,1
//1,32.79726,-117.06936,0.1366835503524,11.749359971945,46.592415422896,1
//2,32.4866,-117.06395,0.70945492969519,12.82745758541,38.483199177109,1
//2,32.4866,-117.06395,0.14189098593904,11.632481284879,38.483199177109,1
//3,32.78162,-117.18077,0.73565125141017,12.521057238345,38.374800946375,1
//3,32.78162,-117.18077,0.14713025028203,11.455575772716,38.374800946375,1
//4,32.78607,-117.14765,0.62945483167057,13.006738974831,40.690836988233,1