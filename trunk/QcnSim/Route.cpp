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

#include "Route.h"

/**
 * @brief Default constructor.
 */
Route::Route(): explicitRoute(), explicitRouteNextHopIndex(), recordedRoute() {
}

/**
 * @brief Constructor with route.
 *
 * @details 
 * RecordThisRoute flag is set to false by default.
 *
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 */
Route::Route(std::vector<std::shared_ptr<Entity>> explicitRoute): explicitRoute(explicitRoute), recordedRoute(), explicitRouteNextHopIndex(0) {
}

/**
 * @brief Get explicit route.
 *
 * @return Explicit route within this object.
 */
std::vector<std::shared_ptr<Entity>> Route::getExplicitRoute() const {
	return explicitRoute;
}

///**
// * @brief Get value of flag recordThisRoute.
// *
// * @return True is route is being recorded; false if route is not being recorded.
// */
//bool Route::isRouteBeingRecorded() const {
//	return recordThisRoute;
//}

/**
 * @brief Get recorded route so far.
 *
 * @return Current recorded route or path travelled by token.
 */
std::vector<std::shared_ptr<Entity>> Route::getRecordedRoute() const {
	return recordedRoute;
}

/**
 * @brief Set explicit route token must follow.
 *
 * @param explicitRoute Explicit route token must follow.
 */
void Route::setExplicitRoute(std::vector<std::shared_ptr<Entity>> explicitRoute) {
	this->explicitRoute = explicitRoute;
	explicitRouteNextHopIndex = 0;
}

///**
// * @brief Set flag to record the route followed by token to TRUE.
// */
//void Route::setRecordThisRoute() {
//	recordThisRoute = true;
//}

///**
// * @brief Unset flag to record the route followed by token (set to FALSE).
// */
//void Route::setDoNotRecordThisRoute() {
//	recordThisRoute = false;
//}

/**
 * @brief Record hop within recorded route.
 *
 * @details 
 * This function should only be executed with corresponding recordThisRoute within Token class is set.
 * Token class must assure that.
 *
 * @param hop Entity object to record in the route.
 */
void Route::addHopToRecordedRoute(std::shared_ptr<Entity> hop) {
	recordedRoute.push_back(hop);
}

/**
 * @brief Gets the next hop from the explicit route vector.
 *
 * @details 
 * This function fetches the next hop within the explicit route vector by using the iterator.
 * It also increments the iterator such that subsequent calls will follow the path.
 * If the end of the vector is reached, this function just returns the last hop.
 * If there is no explicit route, then this function returns nullptr.
 *
 * @return Entity pointer as the next hop.
 */
std::shared_ptr<Entity> Route::getNextHopFromExplicitRoute() {
	if (explicitRoute.empty()) {
		return nullptr; // No explicit route; return nullptr.
	}
	// If index points to end(), then return the last hop.
	if (explicitRouteNextHopIndex >= explicitRoute.size()) {
		return explicitRoute.back();
	}
	size_t currentNextHopIndex = explicitRouteNextHopIndex; // Get next hop index before we increment it.
	++explicitRouteNextHopIndex; // Advance index to hop after next.
	return explicitRoute.at(currentNextHopIndex);
}