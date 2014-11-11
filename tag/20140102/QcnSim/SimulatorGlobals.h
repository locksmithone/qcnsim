/**
 * @author Marcos Portnoi
 * @date April 2013
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

#include <string>
#include <random>

#define CURRENT_ABSOLUTE_TIME 0.0  //!< default currentAbsoluteTime
// Let the default SIMULATION_ABSOLUTE_START_TIME be equal to CURRENT_ABSOLUTE_TIME.
#define SIMULATION_ABSOLUTE_START_TIME 0.0  //!< Indicates the absolute time measurements should consider as the start of the simulation.
#define PRINT_TRACE_FLAG false //!< default printTraceFlag
#define VERSION "2013.08.02" //!< default version string

/**
 * @brief Simulator-scope variables and parameters.
 * 
 * @par Description
 * Simulator-scope variables and parameters will be here.
 * Most important one is currentAbsoluteTime, which indicates the current, clock time of the simulation.
 * It also includes resources to automatically generate unique token IDs and maintain a simulator-unique
 * random number generator engine and seed.
 * 
 * Changelog:
 * 
 *
 */
class SimulatorGlobals {
private:
	double currentAbsoluteTime; //!< Current simulation time.
	double simulationAbsoluteStartTime; //!< Indicates the absolute time measurements should consider as the start of the simulation. This allows a simulation to have a "warm-up" time, which statistics will disregard.  To use, just set this variable to the time after which statistics should be calculated, e.g., if set to 5.0, busy times and utilization measurements before 5.0 will be disregarded (by doing the subtraction currentAbsoluteTime - measurementsAbsoluteStartTime).
	bool printTraceFlag; //!< TRUE:  prints tracing information; FALSE:  does not print tracing information during simulation.
	std::string version; //!< Simulator current version.
	//std::random_device randomDevice; //!< Random device for random generators. // Cannot be done here... move it to cpp implementation.
	std::default_random_engine randomEngine; //!< The Random Engine to be used by traffic generators.
	unsigned int seed; //!< The actual seed to use (or being used) for the random number generator.
	unsigned int tokenInitialId; //!< Initial ID for tokens generated in this simulator. To assure unique IDs, use the function getTokenNextId().
	
	void initializeRandomGeneratorRandomSeed();

public:
	SimulatorGlobals();
	SimulatorGlobals(double currentAbsoluteTime, double simulationAbsoluteStartTime, bool printTraceFlag, std::string version);
	SimulatorGlobals(double currentAbsoluteTime, bool printTraceFlag, std::string version);

	double getCurrentAbsoluteTime() const;
	double getSimulationDuration() const;
	bool isTraceOn() const;
	std::string getVersion() const;
	unsigned int getRandomNumberGeneratorSeed() const;
	void seedRandomNumberGenerator(unsigned int seed);
	void setSimulationAbsoluteStartTime(double simulationAbsoluteStartTime);
	void setCurrentAbsoluteTime(double currentAbsoluteTime);
	unsigned int getTokenNextId();
	std::default_random_engine &getRandomNumberGeneratorEngineInstance();

	/// @todo Tracing is not yet implemented. Each class must implement its own trace routines.
};
