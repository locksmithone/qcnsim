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

#include "Entity.h"
#include "SimulatorGlobals.h"
#include <utility>
#include <memory>

/**
 * @brief Token class.
 * 
 * @par Description
 * Tokens represents the active entities of the system. The dynamic behavior of the system is modeled by the
 * movement of tokens through set of facilities. A token may represent a task in a computer system model, a
 * packet in communication model or memory access in a memory bus subsystem model. A token may reserve (preempt)
 * a facility or schedule activity of various durations. A token can be a single integer (customer id), an structure,
 * (enter time, size, etc.) or another object (such as a packet).
 *
 * The routing of tokens throughout the simulator is done by carefully constructing the chain of events (which event schedules with event)
 * and by putting pertinent information within the token, i.e., the source Entity and destination Entity, or previous Entity, next Entity.
 * Upon processing an event, a processing function can capture, for example, the appropriate facility to which request service for a token.
 */
class Token: public Entity {
public:
	unsigned int id;	//!< Token unique ID.
	int priority;		//!< Token priority. Higher priority, higher number.
	// Reference member variables are driving me nuts with initialization requirements and lifetime... let's drop them and use smart pointers instead.
	// Entity &entity;		//!< Reference to associated Entity object (typically another child of Entity class).
	std::shared_ptr<Entity> associatedEntity;  //!< Reference to associated Entity object (typically another child of Entity class).
	std::shared_ptr<Entity> previous; //!< Previous entity that had this token (for token routing).
	std::shared_ptr<Entity> next; //!< Next entity that will have to process this token (to which entity the token has to be "sent", e.g., for which request service).
	std::shared_ptr<Entity> source; //!< Source entity of this token.
	std::shared_ptr<Entity> destination; //!< Destination entity of this token.

	Token(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination);
	Token(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next);
	Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination);
	Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next);
	~Token();
	
	///// Copy constructor
	//Token(const Token &source);

	///// Move Constructor
	//Token(Token &&source);

	///// Swap function
	//friend void Token::swap(Token &left, Token &right);

	///// Assignment operator
	//Token &operator=(Token right); // rhs as in Right Hand Side.

	///	Comparator
	friend bool Token::operator==(const Token &left, const Token &right);

	/// @todo Do we need a move assignment operator?
};					