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

#include "Link.h"

/**
 * @brief SimplexLink class.
 * 
 * @par Description
 * 
 */
class SimplexLink: public Link {
private:

public:
	SimplexLink(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB, double bandwidth, double propagationDelay, SimulatorGlobals &simulatorGlobals, Scheduler &scheduler, const std::string &name = "");

	std::shared_ptr<Node> getSourceNode() const;
	std::shared_ptr<Node> getDestinationNode() const;
};
