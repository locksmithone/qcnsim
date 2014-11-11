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

/**
 * @mainpage QCNSim - QCN Simulator
 *
 * @section introduction_section Introduction
 * Intro
 *
 * @section section1_section Section 1
 * Section 1
 *
 */

// QcnSim.cpp : Defines the entry point for the console application.
//

#pragma once

#include "Entity.h"
#include "Event.h"
#include "EventType.h"
#include "Facility.h"
#include "FacilityReturnType.h"
#include "Scheduler.h"
#include "SimulatorGlobals.h"
#include "Token.h"
#include "QcnSensorTrafficGenerator.h"
#include "SeismicEventData.h"
#include "SimplexLink.h"
#include "Node.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <iostream>
#include <map>
#include <iomanip>
//#include <vector>