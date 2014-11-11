/**
 * @author Marcos Portnoi
 * @date April 2013
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
 * @brief EventType enum class.
 *
 * @par Description
 * Names for events that will be handled by the Simulator.
 * The events are typically handled in a switch-case strucutre in a main loop.
 */
enum class EventType {
	BEGIN_SIMULATION,							//!< Begin of simulation event.
	REQUEST_SERVICE_FOR_TOKEN_AT_FACILITY,		//!< Request service for a token at a facility.
	REQUEST_SERVICE_FOR_TOKEN_AT_CENTRAL_FACILITY,		//!< Request service for a token at a facility.
	REQUEST_SERVICE_FOR_TOKEN_AT_BACKUP_FACILITY,		//!< Request service for a token at a facility.
	REQUEST_SERVICE_FOR_TOKEN_AT_NODE,			//!< Request "service" for a token at a node (route, application agent, etc.).
	ACTIVATE_BACKUP_FACILITY,					//!< Specific for a test run.
	RELEASE_TOKEN_FROM_SERVICE_AT_FACILITY,		//!< Release a token from service at a facility.
	TRAFFIC_GENERATOR_ARRIVAL,					//!< Arrival of instance of traffic generator.
	END_SIMULATION								//!< End of simulation event.  Should be the last event to occur in the simulation, and the Event Chain should have at least this event for soundness.
};