/**
 * @author Marcos Portnoi
 * @date January 2014
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
#include "Node.h"
#include "SimulatorGlobals.h"
#include "Scheduler.h"
#include "QcnSensorTrafficGenerator.h"
#include "Entity.h"
#include <map>
#include <memory>

/**
 * @brief Topology class.
 * 
 * @par Description
 * This Topology class implements data structures and functions to facilitate management of the network topology part of a QCN simulation.
 *
 * In particular, this class can concentrate vectors of Nodes, Links, traffic generators, etc. A few utility functions might include finding a link
 * given nodeA and nodeB and getters and setters.
 *
 * This is a template for future implementations. Use with wisdom. :)
 *
 */
class Topology {
public:
	std::map<unsigned int, std::shared_ptr<QcnSensorTrafficGenerator>> qcnSensorTrafficGeneratorMap; //!< Map to hold all qcnSensorTrafficGenerators.
	std::map<unsigned int, std::shared_ptr<Node>> nodeMap; //!< Holds all nodes in the network.
	std::map<unsigned int, std::shared_ptr<Link>> linkMap; //!< Map to concentrate all network links.
	std::map<unsigned int, std::vector<std::shared_ptr<Entity>>> explicitRouteMap; //!< Map to concentrate all explicit route objects.

	std::shared_ptr<Link> findLinkByNodeB(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB);
};