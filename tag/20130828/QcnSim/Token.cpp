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
 * @details
 * ID is provided by parameter. AbsoluteGenerationTime is marked as invalid by putting max value of double (since to mark the timestamp, simulatorGlobals object is needed.
 * 
 * @param id Token unique ID.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 */
Token::Token(unsigned int id, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination): 
		id(id), priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), absoluteGenerationTime(DEFAULT_GENERATION_TIME),
		recordThisRoute(false) {
}

/**
 * @brief Constructor with parameters.
 * 
 * @details
 * ID is provided by parameter. AbsoluteGenerationTime is marked as invalid by putting max value of double (since to mark the timestamp, simulatorGlobals object is needed.
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
		id(id), priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), previous(previous), next(next), route(), absoluteGenerationTime(DEFAULT_GENERATION_TIME), 
		recordThisRoute(false) {
}

/**
 * @brief Constructor with parameters (but previous and next), automatic token ID creation.
 *
 * @details
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic token ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic token ID will be obtained.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 */
Token::Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination): 
		priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), route(), recordThisRoute(false) {
	id = simulatorGlobals.getTokenNextId();
	absoluteGenerationTime = simulatorGlobals.getCurrentAbsoluteTime();
}

/**
 * @brief Constructor with parameters, automatic token ID creation.
 *
 * @details
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic token ID, and also the clock timestamp of generation.
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
		priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), previous(previous), next(next), route(), recordThisRoute(false) {
	id = simulatorGlobals.getTokenNextId();
	absoluteGenerationTime = simulatorGlobals.getCurrentAbsoluteTime();
}

/**
 * @brief Constructor with parameters (but previous and next), automatic token ID creation.
 *
 * @details
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic token ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic token ID will be obtained.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 */
Token::Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity,
		std::shared_ptr<Entity> source, std::shared_ptr<Entity> destination, std::vector<std::shared_ptr<Entity>> explicitRoute): 
		priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), route(explicitRoute), recordThisRoute(false) {
	id = simulatorGlobals.getTokenNextId();
	absoluteGenerationTime = simulatorGlobals.getCurrentAbsoluteTime();
}

/**
 * @brief Constructor with parameters, automatic token ID creation.
 *
 * @details
 * This constructor uses SimulatorGlobals object function to obtain a globally unique, automatic token ID, and also the clock timestamp of generation.
 * 
 * @param simulatorGlobals Reference to SimulatorGlobals object, from which an automatic token ID will be obtained.
 * @param priority Token priority. Higher priority, higher number.
 * @param associatedEntity Reference to associated Entity object (typically another child of Entity class).
 * @param source Source entity of this token.
 * @param destination Destination entity of this token.
 * @param previous Previous entity that had this token (for token routing).
 * @param next Next entity that will have to process this token (to which entity the token has to be "sent", e.g., for which request service).
 * @param explicitRoute Vector containing a list of Entity objects representing the route to be followed by a token/PDU.
 */
Token::Token(SimulatorGlobals &simulatorGlobals, int priority, std::shared_ptr<Entity> associatedEntity, std::shared_ptr<Entity> source,
		std::shared_ptr<Entity> destination, std::shared_ptr<Entity> previous, std::shared_ptr<Entity> next, std::vector<std::shared_ptr<Entity>> explicitRoute): 
		priority(priority), associatedEntity(associatedEntity), source(source), destination(destination), previous(previous), next(next), route(explicitRoute),
		recordThisRoute(false) {
	id = simulatorGlobals.getTokenNextId();
	absoluteGenerationTime = simulatorGlobals.getCurrentAbsoluteTime();
}

/**
 * Destructor.
 */
Token::~Token() {
}

/**
 * @brief Set the explicit route for this token and erases previously recorded route.
 *
 * @details
 * The explicit route is a vector of Entity object, typically Node objects.
 * The first Entity object can be the source of the token (a traffic source) or the next Entity after the source.
 * The forwarding function will consider that, if the first route is the source of the token, it must be ignored.
 *
 * @param explicitRoute A vector of Entity objects describing the route, in order, the token is to follow.
 */
void Token::setExplicitRoute(std::vector<std::shared_ptr<Entity>> explicitRoute){
	route = Route(explicitRoute);
}

///**
// * @brief Gets the (reference to) route attached to token.
// *
// * @details
// * Return a reference here is necessary, because other functions might want to modify the Route object within this token, such as
// * routing functions.
// *
// * @return Reference to Route object.
// */
//Route& Token::getRoute() {
//	return route;
//}

/**
 * @brief Gets the generation timestamp of this token.
 *
 * @return Generation timestamp.
 */
double Token::getAbsoluteGenerationTime() const {
	return absoluteGenerationTime;
}

/**
 * @brief Get explicit route.
 *
 * @return Explicit route within this object.
 */
std::vector<std::shared_ptr<Entity>> Token::getExplicitRoute() const {
	return route.getExplicitRoute();
}

/**
 * @brief Get value of flag recordThisRoute.
 *
 * @return True is route is being recorded; false if route is not being recorded.
 */
bool Token::isRouteBeingRecorded() const {
	return recordThisRoute;
}

/**
 * @brief Get recorded route so far.
 *
 * @return Current recorded route or path travelled by token.
 */
std::vector<std::shared_ptr<Entity>> Token::getRecordedRoute() const {
	return route.getRecordedRoute();
}

/**
 * @brief Set flag to record the route followed by token to TRUE.
 */
void Token::setRecordThisRoute() {
	recordThisRoute = true;
}

/**
 * @brief Unset flag to record the route followed by token (set to FALSE).
 */
void Token::setDoNotRecordThisRoute() {
	recordThisRoute = false;
}

/**
 * @brief Record hop within recorded route, if flag is set.
 *
 * @details
 * This function will only record the hop if the recordThisRoute is true.
 * Therefore, it is safe to simply call this function at every hop, without prior checking of the flag.
 *
 * @param hop Entity object to record in the route.
 */
void Token::addHopToRecordedRoute(std::shared_ptr<Entity> hop) {
	if (recordThisRoute) {
		// Flag is true; include hop in recorded route vector.
		route.addHopToRecordedRoute(hop);
	} // If flag is false, do nothing.
}

/**
 * @brief Update previous/next hops from the explicit route vector to prepare the token to be forwarded within a route/path.
 *
 * @details
 * This function updates the previous and next fields to reflect the next hop as indicated by the Route object.
 * After updating, the previous field will contain the current hop, and the next field will contain the next hop to which the token
 * must be forwarded. The token is, thus, ready to be forwarded.
 *
 * @param currentHop The current Entity object at which the token is. It will be populated into the "previous" field.
 */
void Token::updateHopsFromExplicitRoute(std::shared_ptr<Entity> currentHop) {
	// If the currentHop and the nextHop are the same, this might indicate that the token has reached its destination OR
	// that the token is at its source, and the explicit route has the source at the beginning. Treat this here or at a Node class?
	// In this case, after updating, previous and next will have the same value (and possibly destination too).
	this->previous = currentHop;
	// Notice that route is not recorded here. The Node class should call the function to add hop to route upon arrival of a token.
	// Fetch the next hop from Route object attached to token. If it is NULL, then there is no route. Must treat this in caller.
	this->next = route.getNextHopFromExplicitRoute();
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
 * It should suffice to compare the unique IDs, but let's just do it the redundant way.
 * 
 * @param left The Token object to the "left", to be compared with the "right".
 * @param right The Token object to the "right", to be compared with the "left".
 */
bool operator==(const Token &left, const Token &right)  {
	return (left.id == right.id && left.priority == right.priority && left.associatedEntity == right.associatedEntity &&
		left.previous == right.previous && left.next == right.next && left.source == right.source && left.destination == right.destination &&
		left.absoluteGenerationTime == right.absoluteGenerationTime);
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
