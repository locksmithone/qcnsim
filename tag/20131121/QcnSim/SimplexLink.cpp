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

#include "SimplexLink.h"

/**
 * @brief Default constructor.
 *
 * @details 
 * Calls the base class constructor to populate member variables.
 * Also pass the references to this class private members and use them to make nodes more evident.
 *
 * @param nodeA Reference to Node at source. It is nodeA at base class.
 * @param nodeB Reference to Node at destination. It is nodeB at base class.
 * @param bandwidth Bandwidth of this link, typically in bits per second.
 * @param propagationDelay Propagation delay of this link, which is medium-dependent, typically in seconds.
 * @param simulatorGlobals Reference to SimulatorGlobals object.
 * @param scheduler Reference to Scheduler object.
 * @param name Name of this link. Default is "".
 */
SimplexLink::SimplexLink(std::shared_ptr<Node> nodeA, std::shared_ptr<Node> nodeB, double bandwidth, double propagationDelay, SimulatorGlobals &simulatorGlobals,
		Scheduler &scheduler, const std::string &name) : Link(nodeA, nodeB, bandwidth, propagationDelay, simulatorGlobals, scheduler, name) {
}

/**
* @brief Return pointer to source node (one side of link).
*
* @return Pointer to nodeA or source.
*/
std::shared_ptr<Node> SimplexLink::getSourceNode() const {
	return getNodeA();
}

/**
* @brief Return pointer to destination (another side of link).
*
* @return Pointer to nodeB or destination
*/
std::shared_ptr<Node> SimplexLink::getDestinationNode() const {
	return getNodeB();
}
