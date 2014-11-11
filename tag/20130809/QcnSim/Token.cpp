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

#include "Token.h"

/**
 * @brief Constructor with parameters, but previous and next.
 * 
 * @param id Token unique ID.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 */
Token::Token(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination): 
		id(id), priority(priority), associatedEntity(associatedEntity), source(source), destination(destination) {
}

/**
 * @brief Constructor with parameters.
 * 
 * @param id Token unique ID.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 * @param previous Previous entity that had this token (for token routing).
 * @param next Next entity that will have to process this token (to which entity the token has to be "sent", e.g., for which request service).
 */
Token::Token(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next): 
		id(id), priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), previous(previous), next(next) {
}

/**
 * @brief Constructor with parameters (but previous and next), automatic token ID creation.
 *
 * @details
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic token ID.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic token ID will be obtained.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 */
Token::Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination): 
		priority(priority), associatedEntity(associatedEntity), source(source), destination(destination) {
			id = simulatorGlobals.getTokenNextId();
}

/**
 * @brief Constructor with parameters, automatic token ID creation.
 *
 * @details
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic token ID.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic token ID will be obtained.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 * @param previous Previous entity that had this token (for token routing).
 * @param next Next entity that will have to process this token (to which entity the token has to be "sent", e.g., for which request service).
 */
Token::Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next): 
		priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), previous(previous), next(next) {
			id = simulatorGlobals.getTokenNextId();
}


/**
 * Destructor.
 */
Token::~Token() {
}
//
///**
// * Copy constructor.
// * 
// * @param source Token object that will be copied.
// */
//Token::Token(const Token &source): id(source.id), priority(source.priority), entity(source.entity) {
//}
//
///**
// * Move constructor.
// *
// * @details
// * This is used per The Big Three Rule, or The Four or Five Rule (http://stackoverflow.com/questions/4172722/what-is-the-rule-of-three).
// *
// * @param source The "source" Token object that will have its resources moved to the current object. The "source" object will likely be destroyed afterwards.
// */
//Token::Token(Token &&source): id(0), priority(0), entity(Message()) { // We use a dummy Entity object here (the Message) just to initialize the reference to Entity object.
//	swap(*this, source);
//}
//
///**
// * Operator = (assignment) overloading.
// *
// * @details
// * This is the overloading of the assignment operator for use with token.
// * Necessary because a member of the Token class is a reference (&entity).
// * This is used per The Big Three Rule, or The Four or Five Rule (http://stackoverflow.com/questions/4172722/what-is-the-rule-of-three).
// * 
// * @param right The Token object to the right of the assignment operator.
// */
//Token &Token::operator=(Token right) { // rhs as in Right Hand Side.
//	swap(*this, right);
//	return *this;
//}
//
/**
 * Operator == (comparison) overloading.
 *
 * @details
 * This is the overloading of the comparison operator for use with token.  The result should be TRUE
 * if the left token has the same contents as the right token.
 * 
 * @param left The Token object to the "left", to be compared with the "right".
 * @param right The Token object to the "right", to be compared with the "left".
 */
bool operator==(const Token &left, const Token &right)  {
	return (left.id == right.id && left.priority == right.priority && left.associatedEntity == right.associatedEntity &&
		left.previous == right.previous && left.next == right.next && left.source == right.source && left.destination == right.destination);
}
//
///**
// * Swap function
// *
// * @details
// * This is used per The Big Three Rule, or The Four or Five Rule (http://stackoverflow.com/questions/4172722/what-is-the-rule-of-three).
// * It is an implementation of the Copy-and-Swap idiom.
// *
// * @param left Token object to the left that will be swapped.
// * @param right Token object to the right that will be swapped.
// */
//void swap(Token &left, Token &right) {
//	std::swap(left.entity, right.entity);
//	std::swap(left.id, right.id);
//	std::swap(left.priority, right.priority);
//	// Now right is left, and left is right.
//}
