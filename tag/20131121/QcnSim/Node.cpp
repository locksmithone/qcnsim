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

#include "Node.h"

/**
 * @brief Default constructor.
 *
 * @details 
 * Sets all member variables (statistics) to zero.
 *
 * @param simulatorGlobals SimulatorGlobals object.
 */
Node::Node(SimulatorGlobals &simulatorGlobals): receivedBytesCount(0), receivedPdusOrTokensCount(0), forwardedPdusOrTokensCount(0), forwardedBytesCount(0), droppedPdusOrTokensCount(0),
		lastDelay(0.0), sumDelay(0.0), meanDelay(0.0), lastJitter(0.0), sumJitter(0.0), meanJitter(0.0), previousDelay(0.0), simulatorGlobals(simulatorGlobals) {
}

/**
 * @brief Get received bytes count.
 *
 * @details 
 * The size, in bytes, of a PDU depends on the protocol layer; it is, therefore, simulation/implementation dependent.
 *
 * @return Received bytes count.
 */
unsigned int Node::getReceivedBytesCount() const {
	return receivedBytesCount;
}

/**
 * @brief Get received PDUs count.
 *
 * @return Received PDUs count.
 */
unsigned int Node::getReceivedPdusOrTokensCount() const {
	return receivedPdusOrTokensCount;
}

/**
 * @brief Get forwarded PDUs count.
 *
 * @return Forwarded PDUs count.
 */
unsigned int Node::getForwardedPdusOrTokensCount() const {
	return forwardedPdusOrTokensCount;
}

/**
 * @brief Get forwarded PDUs count.
 *
 * @return Forwarded PDUs count.
 */
unsigned int Node::getForwardedBytesCount() const {
	return forwardedBytesCount;
}

/**
 * @brief Get dropped PDUs count.
 *
 * @return Dropped PDUs count.
 */
unsigned int Node::getDroppedPdusOrTokensCount() const {
	return droppedPdusOrTokensCount;
}

/**
 * @brief Get last PDU measured delay.
 *
 * @return Last PDU measured delay.
 */
double Node::getLastPduOrTokenDelay() const {
	return lastDelay;
}

/**
 * @brief Get Sum of all measured PDU delays.
 *
 * @return Sum of all measured PDU delays.
 */
double Node::getSumPduOrTokenDelay() const {
	return sumDelay;
}

/**
 * @brief Get mean of all measured PDU delays.
 *
 * @return Mean of all measured PDU delays.
 */
double Node::getMeanPduOrTokenDelay() const {
	return meanDelay;
}

/**
 * @brief Get jitter (difference between delays) as measured between last two PDUs.
 *
 * @return Jitter.
 */
double Node::getLastPduOrTokenJitter() const {
	return lastJitter;
}

/**
 * @brief Get sum of all measured jitters (difference between delays).
 *
 * @return Sum of all measured jitters (difference between delays).
 */
double Node::getSumPduOrTokenJitter() const {
	return sumJitter;
}

/**
 * @brief Get Mean of all measured jitters (difference between delays).
 *
 * @return Mean of all measured jitters (difference between delays).
 */
double Node::getMeanPduOrTokenJitter() const {
	return meanJitter;
}

/**
 * @brief Process the token by updating statistics and other procedures. Then, "forward" the token by changing its previous/next hops according to the attached route.
 *
 * @details 
 * Here, the token is received by the node. Statistics are updated, and other maintenance procedures are taken.
 * The token is "forwarded" according to the attached route by changing previous/next pointers in the token.
 * Also, it is checked whether this node is the destination node, in which case the token is discarded.
 * Route is also recorded here (if the corresponding flag within the Token object is set).
 *
 * This Node assumes it is the current node within which the token is. If it is not the current node, further outcomes are undefined.
 *
 * @param token Token currently at this node, i.e., token just arrived at the node.
 */
NodeReturnType Node::processAndForward(std::shared_ptr<Token> token) {
	// No check for TTL needed in Token, just for PDUs.

	// Is this the destination node AND the first time the token arrives here? If true, update stats and do nothing else.
	if (token->destination.get() == this && token->previous.get() != this) { // Need to compare addresses here! Or maybe a comparator? Nah.
		// Call the record route function; the hop will only be effectively recorded if the recordRoute is set within the Route object.
		// Calling this function without prior checking of the flag is therefore safe.
		token->addHopToRecordedRoute(shared_from_this()); // need smart pointer of self reference here!
		updateArrivalStatistics(token);
		// Update token previous field to this node. Leave other fields untouched.
		token->previous = shared_from_this();
		return NodeReturnType::FINAL_DESTINATION;
	} else if (token->destination.get() == this && token->previous.get() == this) {
		// If this node is the destination and token has been here (received) just before, do nothing.
		return NodeReturnType::FINAL_DESTINATION;
	} else {
		// This is not the destination node. Forward the token to the next hop by modifying the appropriate token fields.
		// Do not deal with TTL here; leave it for PDU types.
		token->addHopToRecordedRoute(shared_from_this()); // need smart pointer of self reference here!
		// Sanity check; if the return type is not PDU_ROUTE_UPDATED, there is some inconsistency in the route table or unpredicted bug.
		if (updateForwardHops(token) != NodeReturnType::PDU_ROUTE_UPDATED) {
			std::cout << "Node::processAndForward(token): Inconsistency in routing path. Aborting..." << std::endl;
			exit(1);
		}
		updateArrivalStatistics(token);
		updateForwardingStatistics(token);
		return NodeReturnType::PDU_ROUTE_UPDATED;
	}
}

/**
 * @brief Process the PDU by updating statistics and other procedures. Then, "forward" the PDU by changing its previous/next hops according to the attached route.
 *
 * @details 
 * Here, the PDU is received by the node. Statistics are updated, and other maintenance procedures are taken.
 * The PDU is "forwarded" according to the attached route by changing previous/next pointers in the PDU.
 * Also, it is checked whether this node is the destination node, in which case the PDU is discarded.
 * Route is also recorded here (if the corresponding flag within the Token object is set).
 *
 * This Node assumes it is the current node within which the PDU is. If it is not the current node, further outcomes are undefined.
 *
 * @param pdu PDU currently at this node, i.e., PDU just arrived at the node.
 */
NodeReturnType Node::processAndForward(std::shared_ptr<ProtocolDataUnit> pdu) {
	// Check for TTL; if it is zero (or less), discard PDU, record this hop to route (only if it was not recorded before), do nothing else.
	if (pdu->getTtl() <= 0) {
		if (pdu->getRecordedRoute().back().get() != this) {
			// Only record the hop if it is not already there (case in which the explicit route has this hop more than once in a sequence).
			pdu->addHopToRecordedRoute(shared_from_this());
		}
		++droppedPdusOrTokensCount;
		return NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED;
	}
	// Is this the destination node AND the first time the pdu arrives here? If true, update stats and do nothing else.
	if (pdu->destination.get() == this && pdu->previous.get() != this) { // Need to compare addresses here! Or maybe a comparator? Nah.
		// Call the record route function; the hop will only be effectively recorded if the recordRoute is set within the Route object.
		// Calling this function without prior checking of the flag is therefore safe.
		pdu->addHopToRecordedRoute(shared_from_this()); // need smart pointer of self reference here!
		updateArrivalStatistics(pdu);
		// Update pdu previous field to this node. Leave other fields untouched.
		pdu->previous = shared_from_this();
		return NodeReturnType::FINAL_DESTINATION;
	} else if (pdu->destination.get() == this && pdu->previous.get() == this) {
		// If this node is the destination and pdu has been here (received) just before, do nothing.
		return NodeReturnType::FINAL_DESTINATION;
	} else { // Forward.
		// This is not the destination node. Forward the pdu to the next hop by modifying the appropriate pdu fields.
		// Do not deal with TTL here; leave it for PDU types.
		pdu->addHopToRecordedRoute(shared_from_this()); // need smart pointer of self reference here!
		// Sanity check; if the return type is not PDU_ROUTE_UPDATED, there is some inconsistency in the route table or unpredicted bug.
		if (updateForwardHops(pdu) != NodeReturnType::PDU_ROUTE_UPDATED) {
			std::cout << "Node::processAndForward(PDU): Inconsistency in routing path. Aborting..." << std::endl;
			exit(1);
		}
		pdu->decrementTtl(); // Decrement the TTL in egress.
		updateArrivalStatistics(pdu);
		updateForwardingStatistics(pdu);
		return NodeReturnType::PDU_ROUTE_UPDATED;
	}
}

/**
 * @brief Update Node statistics upon arrival of token.
 *
 * @details 
 * These statistics should only be updated if the token was not discarded due to TTL exceeded, for example.
 */
void Node::updateArrivalStatistics(std::shared_ptr<Token> token) {
	// Should tokens generated from a generator attached to this node still be included in the statistics?
	previousDelay = lastDelay; // Save last measured delay to allow jitter calculations for the next token received here.
	++receivedPdusOrTokensCount;
	//receivedBytesCount +=
	lastDelay = simulatorGlobals.getCurrentAbsoluteTime() - token->getAbsoluteGenerationTime();
	sumDelay += lastDelay;
	meanDelay = sumDelay / receivedPdusOrTokensCount;
	// Perform jitter calculations only if there is more than one token received here.
	if (receivedPdusOrTokensCount > 1) {
		lastJitter = lastDelay - previousDelay;
		sumJitter += lastJitter;
		meanJitter = sumJitter / (receivedPdusOrTokensCount - 1); // Number of measured jitters is equal to count of tokens - 1.
	}
}

/**
 * @brief Update Node statistics upon forwarding of token.
 *
 * @details 
 * These statistics should only be updated if the token is being forwarded.
 */
void Node::updateForwardingStatistics(std::shared_ptr<Token> token) {
	++forwardedPdusOrTokensCount;
}

/**
 * @brief Update Node statistics upon arrival of PDU.
 *
 * @details 
 * These statistics should only be updated if the PDU was not discarded due to TTL exceeded, for example.
 */
void Node::updateArrivalStatistics(std::shared_ptr<ProtocolDataUnit> pdu) {
	// Should PDUs generated from a generator attached to this node still be included in the statistics?
	previousDelay = lastDelay; // Save last measured delay to allow jitter calculations for the next PDU received here.
	++receivedPdusOrTokensCount;
	receivedBytesCount += pdu->getPduSize();
	lastDelay = simulatorGlobals.getCurrentAbsoluteTime() - pdu->getAbsoluteGenerationTime();
	sumDelay += lastDelay;
	meanDelay = sumDelay / receivedPdusOrTokensCount;
	// Perform jitter calculations only if there is more than one PDU received here.
	if (receivedPdusOrTokensCount > 1) {
		lastJitter = lastDelay - previousDelay;
		sumJitter += lastJitter;
		meanJitter = sumJitter / (receivedPdusOrTokensCount - 1); // Number of measured jitters is equal to count of PDUs - 1.
	}
}

/**
 * @brief Update Node statistics upon forwarding of PDU.
 *
 * @details 
 * These statistics should only be updated if the PDU is being forwarded.
 */
void Node::updateForwardingStatistics(std::shared_ptr<ProtocolDataUnit> pdu) {
	++forwardedPdusOrTokensCount;
	forwardedBytesCount += pdu->getPduSize();
}

/**
 * @brief Update token's or PDU's hop fields to prepare it for forwarding, according to explicit route attached to token.
 *
 * @details 
 * This function calls the token member function to update token's previous and next fields, to prepare it for forwarding.
 * If the next hop in the explicit route is the current node, then understand that the current node might the the source of the token. Then, just
 * ignore and retrieve the next hop from the list of explicit route.
 * This behavior allows the user to construct an explicit route that begins with the source hop. The first forwarding would thus forward the node
 * the node at which it is currently; this function predicts that case and just ignores that next hop, and fetches the one after.
 * However, if this case results from some inconsistenty in the explicit route list, then the outcome here is undefined.
 * If no next hop exists, then this function returns the appropriate NodeReturnType.
 *
 * This function should work for Token and ProtocolDataUnit class objects.
 *
 * @return NodeReturnType indicating whether the operation was successful, or not.
 */
NodeReturnType Node::updateForwardHops(std::shared_ptr<Token> token) {
	// Sanity check: if next == destination, there is no forwarding to be done! The token has reached its destination.
	if (token->next.get() == token->destination.get()) {
		return NodeReturnType::FINAL_DESTINATION;
	}
	// Need to check whether next hop is the current hop.
	token->updateHopsFromExplicitRoute(shared_from_this());
	if (this == token->next.get()) { // Indicates that the route table begins with the source node, or there is some inconsistency...
		updateForwardHops(token);  // Recursion. Really? Disaster?
	}
	return NodeReturnType::PDU_ROUTE_UPDATED;
}
