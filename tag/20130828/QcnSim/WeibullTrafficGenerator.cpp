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

#include "WeibullTrafficGenerator.h"

/**
 * Constructor with parameter.
 *
 * @details
 * The seed for the random number generator is controlled by SimulatorGlobals. If none is ever specified, random_device output is used as seed.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, to get random generator info.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param lambda Scale parameter of the Weibull distribution.
 * @param kappa Shape parameter of the Weibull distribution.
 */
WeibullTrafficGenerator::WeibullTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double lambda, double kappa): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority), lambda(lambda), kappa(kappa) {
}

/**
 * Constructor with parameters, seed as parameter.
 *
 * @param simulatorGlobals Reference to SimulatorGlobals object, to get random generator info.
 * @param scheduler Reference to Scheduler object, such that traffic generators may schedule their events.
 * @param eventType Event type that this generator will produce.
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param source Entity to which the generator is attached (to which tokens are generated).
 * @param destination Destination entity, to which the generated tokens will be sent.
 * @param priority Token priority. Higher priority, higher number.
 * @param lambda Scale parameter of the Weibull distribution.
 * @param kappa Shape parameter of the Weibull distribution.
 * @param seed Seed to the default random number generator, to be passed to SimulatorGlobals object.
 */
WeibullTrafficGenerator::WeibullTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double lambda, double kappa, unsigned int seed): TrafficGenerator(simulatorGlobals, scheduler, eventType, tokenContents,
		source, destination, priority, seed), lambda(lambda), kappa(kappa) {
	//this->tau = tau;
	//exponentialDistribution.;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses member variable tokenContents.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent() {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent();
	if (token != NULL) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses parameter tokenContents.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents);
	if (token != NULL) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses member variable tokenContents and parameter explicit route.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(explicitRoute);
	if (token != NULL) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Creates an instance of Weibull traffic event.
 *
 * @details
 * Calls parent function and complements with event generation and Weibull random variate.
 * Uses parameter tokenContents and explicit route.
 *
 * @param tokenContents Reference to Entity object that will be carried by the token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 *
 * @return Token that was generated, per the class constructor parameters and if generator is On. Otherwise, returns NULL.
 */
std::shared_ptr<Token> WeibullTrafficGenerator::createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute) {
	// Creates a token and assigns it class member values and a unique ID obtained from simulatorGlobals.
	std::shared_ptr<Token> token = TrafficGenerator::createInstanceTrafficEvent(tokenContents, explicitRoute);
	if (token != NULL) {
		scheduler.schedule(Event(generateWeibullVariate(), eventType, token));
	}
	return token;
}

/**
 * Generate a Weibull distribution variate based on member variables lambda and kappa.
 *
 * @details
 * The Weibull variate is generated using C++11 Random library and uses the random number generator obtained from SimulatorGlobals,
 * such that its global seed can be utilized.
 *
 * @return Weibull distribution variate based on member variables lambda and kappa.
 */
double WeibullTrafficGenerator::generateWeibullVariate() {
	// Have not seen different behavior with or without static below, per unit testing.
	// The pseudorandom sequence seems to be unique per simulation, not per generator; the former is the expected behavior.
	std::weibull_distribution<double> weibullGenerator(kappa, lambda); /// @todo Must check whether this is the correct parameter order for the Weibull constructor!
	return weibullGenerator(simulatorGlobals.getRandomNumberGeneratorEngineInstance());
}

/**
 * Returns lambda (scale parameter).
 *
 * @return Lambda.
 */
double WeibullTrafficGenerator::getLambda() const {
	return lambda;
}

/**
 * Returns kappa (shape parameter).
 *
 * @return Kappa.
 */
double WeibullTrafficGenerator::getKappa() const {
	return kappa;
}