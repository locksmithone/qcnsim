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

#include "SimulatorGlobals.h"

/**
 * Default Constructor
 */
SimulatorGlobals::SimulatorGlobals(): currentAbsoluteTime(CURRENT_ABSOLUTE_TIME), simulationAbsoluteStartTime(SIMULATION_ABSOLUTE_START_TIME), printTraceFlag(PRINT_TRACE_FLAG), version(VERSION), tokenInitialId(0) {
	initializeRandomGeneratorRandomSeed();
}

/**
 * Constructor with parameters.
 *
 * @param currentAbsoluteTime Current simulation time.
 * @param simulationAbsoluteStartTime Indicates the absolute time measurements should consider as the start of the simulation.
 * @param printTraceFlag TRUE:  prints tracing information; FALSE:  does not print tracing information during simulation.
 * @param version Simulator current version.
 */
SimulatorGlobals::SimulatorGlobals(double currentAbsoluteTime, double simulationAbsoluteStartTime, bool printTraceFlag, std::string version): currentAbsoluteTime(currentAbsoluteTime), simulationAbsoluteStartTime(simulationAbsoluteStartTime), printTraceFlag(printTraceFlag), version(version), tokenInitialId(0) {
	initializeRandomGeneratorRandomSeed();
}

/**
 * Constructor with parameters.
 *
 * @details 
 * Here, simulationAbsoluteStartTime defaults to currentAbsoluteTime.
 *
 * @param currentAbsoluteTime Current simulation time.
 * @param printTraceFlag TRUE:  prints tracing information; FALSE:  does not print tracing information during simulation.
 * @param version Simulator current version.
 */
SimulatorGlobals::SimulatorGlobals(double currentAbsoluteTime, bool printTraceFlag, std::string version): currentAbsoluteTime(currentAbsoluteTime), simulationAbsoluteStartTime(currentAbsoluteTime), printTraceFlag(printTraceFlag), version(version), tokenInitialId(0) {
	initializeRandomGeneratorRandomSeed();
}

/**
 * Initializes the Random Number Generator with a Random Seed.
 *
 */
void SimulatorGlobals::initializeRandomGeneratorRandomSeed() {
	std::random_device randomDevice; // Moved from class member variable to here; it seems this must be instantiated and the class constructor is not doing it.
	this->seed = randomDevice();
	this->randomEngine.seed(this->seed); // Seeds Random Engine with random seed.
}

/**
 * Returns current absolute time.
 *
 * @return Current absolute time.
 */
double SimulatorGlobals::getCurrentAbsoluteTime() const {
	return currentAbsoluteTime;
}

/**
 * Returns a reference to the random number generator engine defined here.
 *
 * @details 
 * Typically for use by traffic generators, and allowing for keeping a unique seed per simulation.
 *
 * @return Reference to the random number generator.
 */
std::default_random_engine &SimulatorGlobals::getRandomNumberGeneratorEngineInstance() {
	return randomEngine;
}

/**
 * Returns the simulation duration in time.
 *
 * @details 
 * The simulation duration is the current absolute time minus the simulation absolute start time.  If the simulation start time
 * is different than zero (when the simulation typically starts), then the time elapsed between zero and simulation duration is
 * considered the "warm-time" period and will be discarded when computing certain statistics, such as facility utilization.
 *
 * @return Simulation duration.
 */
double SimulatorGlobals::getSimulationDuration() const {
	return currentAbsoluteTime - simulationAbsoluteStartTime;
}

/**
 * Returns tracing flag status.
 *
 * @return True if tracing is on; false if tracing is off.
 */
bool SimulatorGlobals::isTraceOn() const {
	return printTraceFlag;
}

/**
 * Returns the simulator version.
 *
 * @return Version.
 */
std::string SimulatorGlobals::getVersion() const {
	return version;
}

/**
 * Returns the current random generator seed.
 *
 * @return seed.
 */
unsigned int SimulatorGlobals::getRandomNumberGeneratorSeed() const {
	return seed;
}

/**
 * Sets the simulation absolute start time.
 *
 * @details 
 * The simulation duration is the current absolute time minus the simulation absolute start time.  If the simulation start time
 * is different than zero (when the simulation typically starts), then the time elapsed between zero and simulation duration is
 * considered the "warm-time" period and will be discarded when computing certain statistics, such as facility utilization.
 */
void SimulatorGlobals::setSimulationAbsoluteStartTime(double simulationAbsoluteStartTime) {
	this->simulationAbsoluteStartTime = simulationAbsoluteStartTime;
}

/**
 * Sets seed for the random number generator engine and seeds the random number generator.
 *
 * @details 
 * The purpose of this function is basically allow for setting a fixed seed for the random number generator, such that a deterministic
 * pseudorandom sequence is established. This might be useful for debugging purposes, or to reproduce simulator runs from the same
 * initial parameters.
 */
void SimulatorGlobals::seedRandomNumberGenerator(unsigned int seed) {
	this->seed = seed; // Sets the seed.
	this->randomEngine.seed(this->seed); // Seeds the random engine.
}

/**
 * Sets the simulation current absolute time.
 *
 * @details 
 * The current absolute time is the "simulation clock". This function sets this clock, typically called from a "caused" event in the
 * event chain.
 */
void SimulatorGlobals::setCurrentAbsoluteTime(double currentAbsoluteTime) {
	this->currentAbsoluteTime = currentAbsoluteTime;
}

/**
 * Returns the next unique token ID.
 *
 * @details 
 * This function generates incremental token IDs. To assure uniqueness for one instance of simulation, always use this function to generate token IDs.
 *
 * @return Unique, incremental token ID.
 */
 unsigned int SimulatorGlobals::getTokenNextId() {
	return ++tokenInitialId;
}
