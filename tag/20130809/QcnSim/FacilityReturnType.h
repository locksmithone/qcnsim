/**
 * @author Marcos Portnoi
 * @date May 2013
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

/**
 * @brief Facility Return Type enum class.
 *
 * @par Description
 * Types of return from certain functions within Facility class.
 */
enum class FacilityReturnType {
	TOKEN_PUT_IN_SERVICE,	//!< Token was successfuly put in service by facility.
	TOKEN_ENQUEUED,			//!< Facility was busy; token was enqueued.
	FACILITY_DOWN,			//!< Facility is down.  Token was dropped (or enqueued, depending on the implementation of function Facility::request).
	TOKEN_RELEASED_DEQUEUED_SERVICE_EVENT_SCHEDULED,  //!< Token was released from service; token queue was not empty, token at head of queue was scheduled for service at this facility.
	TOKEN_RELEASED_QUEUE_EMPTY,  //!< Token was released from service. No token in queue for servicing.
	TOKEN_NOT_FOUND,		//!< Token was not found (typically for release) in this facility (service or queue). This is an error condition.
	NOT_IMPLEMENTED			//!< This function has not been implemented yet.
};