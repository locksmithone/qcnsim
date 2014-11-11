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
#include "Route.h"
#include <utility>
#include <memory>

#define DEFAULT_GENERATION_TIME std::numeric_limits<double>::max() // Hard-coded default generation time.

/**
 * @brief Token class.
 * 
 * @par Description
 * Tokens represents the active entities of the system. The dynamic behavior of the system is modeled by the
 * movement of tokens through set of facilities. A token may represent a task in a computer system model, a
 * packet in communication model or memory access in a memory bus subsystem model. A token may reserve (preempt)
 * a facility or schedule activity of various durations.
 *
 * The routing of tokens throughout the simulator is done by carefully constructing the chain of events (which event schedules with event)
 * and by putting pertinent information within the token, i.e., the source Entity and destination Entity, or previous Entity, next Entity.
 * Upon processing an event, a processing function can capture, for example, the appropriate facility to which request service for a token.
 * This class, the Route class, and the Node class implement routing resources.
 */
class Token: public Entity {
protected:
	Route route; //!< Route object to provide explicit routing for the token, or to record followed routes.
	double absoluteGenerationTime;  //!< Absolute generation timestamp for this token (i.e., the clock time this token was generated). Needs SimulatorGlobals object to set; if constructor without SimulatorGlobals object is used, then this member will have a std::numeric_limits<double>::max() to denote an invalid timestamp.
	bool recordThisRoute; //!< True: route followed by token/PDU must be recorded. False: do not record route followed by token/PDU.

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
	Token(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination,
		std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next);
	Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination);
	Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next);
	Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::vector<std::shared_ptr<Entity>> explicitRoute);
	Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, std::vector<std::shared_ptr<Entity>> explicitRoute);
	~Token();

	virtual void setExplicitRoute(std::vector<std::shared_ptr<Entity>> explicitRoute);
	//Route& getRoute();  // Route object within the Token cannot be accessible from the outside, bypassing member functions!
	virtual double getAbsoluteGenerationTime() const;

	virtual std::vector<std::shared_ptr<Entity>> getExplicitRoute() const;
	virtual bool isRouteBeingRecorded() const;
	virtual std::vector<std::shared_ptr<Entity>> getRecordedRoute() const;
	virtual void setRecordThisRoute();
	virtual void setDoNotRecordThisRoute();
	virtual void addHopToRecordedRoute(std::shared_ptr<Entity> hop);
	virtual void updateHopsFromExplicitRoute(std::shared_ptr<Entity> currentHop);
	
	///// Copy constructor
	//Token(const Token &source);

	///// Move Constructor
	//Token(Token &&source);

	///// Swap function
	//friend void Token::swap(Token &left, Token &right);

	///// Assignment operator
	//Token &operator=(Token right); // rhs as in Right Hand Side.

	///	Comparator, non-member function.
	friend bool operator==(const Token &left, const Token &right);

	/// Not-equal operator, non-member function.
	friend bool operator!=(const Token &left, const Token &right);

	/// @todo Do we need a move assignment operator?
};					