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

#include "TrafficGenerator.h"
#include "EventType.h"
#include <memory>

/**
 * @brief ExponentialTrafficGenerator class.
 * 
 * @par Description
 * This class implements an exponential traffic generator. Events generated from this class will be distributed in time according to an exponential probability distribution:
 * times will be exponentially distributed, events will "arrive" according to a Poisson probability distribution.
 *
 * f(x;tau) = (1/tau)*exp(-(1/tau)*x), for x >= 0.
 * Or, f(x;lambda) = lambda*exp(-lambda*x), for x >= 0.
 *
 * where lambda is the arrival rate (from Poisson) and tau is the interarrival time, or 1/lambda.
 *
 */
class ExponentialTrafficGenerator: public TrafficGenerator {
private:
	double tau;  //!< Mean interarrival time for exponential probability distribution.
	//std::exponential_distribution<double> exponentialDistribution; //!< Exponential probability distribution variate generator.
	double generateExponentialVariate();
	
public:
	ExponentialTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double tau);
	ExponentialTrafficGenerator(SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, EventType eventType, std::shared_ptr<Entity> tokenContents,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, int priority, double tau, unsigned int seed);

	std::shared_ptr<Token> createInstanceTrafficEvent() override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::vector<std::shared_ptr<Entity>> explicitRoute) override;
	std::shared_ptr<Token> createInstanceTrafficEvent(std::shared_ptr<Entity> tokenContents, std::vector<std::shared_ptr<Entity>> explicitRoute) override;
	double getTau() const;
};
