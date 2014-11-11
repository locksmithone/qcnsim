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

#include "Entity.h"
#include <memory>
#include <vector>

/**
 * @brief Route class.
 * 
 * @par Description
 * This class models routes within a network, to be followed by tokens or by PDUs. A route is basically a list of Entity objects a
 * token will follow throughout the network of facilities, in a low-level point of view.
 * In a high-level point of view, a token/PDU will follow nodes and links, each of these Entities. A typical route will indicate nodes only, however.
 * High-level functions must abstract the nodes and populate links to complete a route.
 */
class Route {
private:
	std::vector<std::shared_ptr<Entity>> explicitRoute; //!< Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
	//std::vector<std::shared_ptr<Entity>>::iterator explicitRouteIterator; //!< Iterator pointing to the next Entity to which the token/PDU is to be forwarded.
	size_t explicitRouteNextHopIndex; //!< Index pointing to the next Entity to which the token/PDU is to be forwarded.
	// This flag belongs to Token, not here!
	//bool recordThisRoute; //!< True: route followed by token/PDU must be recorded. False: do not record route followed by token/PDU.
	std::vector<std::shared_ptr<Entity>> recordedRoute; //!< Route that was followed by token/PDU, currently being recorded.

// Maybe everything should be private, with friend class Token!
//public:
	Route();
	Route(std::vector<std::shared_ptr<Entity>> explicitRoute);

	std::vector<std::shared_ptr<Entity>> getExplicitRoute() const;
	//bool isRouteBeingRecorded() const;
	std::vector<std::shared_ptr<Entity>> getRecordedRoute() const;
	void setExplicitRoute(std::vector<std::shared_ptr<Entity>> explicitRoute);
	//void setRecordThisRoute();
	//void setDoNotRecordThisRoute();
	void addHopToRecordedRoute(std::shared_ptr<Entity> hop);
	std::shared_ptr<Entity> getNextHopFromExplicitRoute();

	friend class Token;

};			