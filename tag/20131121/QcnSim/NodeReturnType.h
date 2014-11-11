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

/**
 * @brief Node Return Type enum class.
 *
 * @par Description
 * Types of return from certain functions within Node class.
 */
enum class NodeReturnType {
	PDU_ROUTE_UPDATED,			//!< Node is not final destination; route information was updated within PDU/token, to be forwarded. Stats updated.
	FINAL_DESTINATION,			//!< This node is the final destination. Stats updated, nothing else to do in node.
	TTL_EXCEEDED_PDU_DISCARDED,	//!< TTL for this PDU has exceeded. Nothing done.
	ROUTE_NOT_FOUND,			//!< Problem found in updating forwarding information; no next hop, or empty Route object.
	NOT_IMPLEMENTED				//!< This function has not been implemented yet.
};