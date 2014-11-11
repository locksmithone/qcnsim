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

/**
* @brief Link Return Type enum class.
*
* @par Description
* Types of return from certain functions within Link (and derived) class.
*/
enum class LinkReturnType {
	PDU_IN_TRANSMISSION_NEXT_EVENT_SCHEDULED,	//!< PDU was successfully put into transmission (serviced) at Link.
	LINK_BUSY_PDU_ENQUEUED,			//!< Link (transmission server) was busy; PDU was enqueued.
	LINK_DOWN_PDU_DROPPED,			//!< Link is down.  PDU was dropped (or enqueued, depending on the implementation of function Facility::request).
	LINK_BUSY_QUEUE_FULL_PDU_DROPPED, //!< The queue is full (has reached the imposed limit); PDU for which service was requested was dropped.
	PDU_IN_TRANSIT_NEXT_EVENT_SCHEDULED,			//!< PDU is in transit within link medium (and within inTransitQueue).
	ERROR_LINK_DOES_NOT_CONNECT_NODES,	//!< The nodes within PDU's previous and next fields are not connected through this link. PDU cannot travel here (inconsistency).
	PDU_PROPAGATED,			//!< PDU has been propagated (transited) through the link medium and removed from inTransitQueue (if PDU was there).
	NOT_IMPLEMENTED			//!< This function has not been implemented yet.
};