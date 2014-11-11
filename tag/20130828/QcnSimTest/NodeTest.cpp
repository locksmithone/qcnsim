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

#include "NodeTest.h"

/**
 * Constructor.
 *
 * Do initializations here.
 */
NodeTest::NodeTest(): simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "NodeTest")), 
		scheduler(Scheduler(simulatorGlobals)) {
}

/// Construct Node, test default values.
TEST_F(NodeTest, Constructor) {
	Node node(simulatorGlobals);
	EXPECT_EQ(0, node.getReceivedBytesCount());
	EXPECT_EQ(0, node.getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, node.getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, node.getForwardedBytesCount());
	EXPECT_EQ(0, node.getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, node.getDroppedPdusOrTokensCount());
	EXPECT_EQ(0.0, node.getLastPduOrTokenDelay());
	EXPECT_EQ(0.0, node.getSumPduOrTokenDelay());
	EXPECT_EQ(0.0, node.getMeanPduOrTokenDelay());
	EXPECT_EQ(0.0, node.getLastPduOrTokenJitter());
	EXPECT_EQ(0.0, node.getSumPduOrTokenJitter());
	EXPECT_EQ(0.0, node.getMeanPduOrTokenJitter());
}

/// Test Node explicit route capabilities.
TEST_F(NodeTest, ExplicitRoute) {
	// Build 5 nodes. Will test routing between them using explicit route attached to tokens. No need for links or facilities here.
	//std::shared_ptr<Message> dummyContents(new Message("This is a dummy Token contents."));
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> explicitRouteGood;
	std::vector<std::shared_ptr<Entity>> explicitRouteBad;
	for (int i = 0; i < 5; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Push each node in a certain order to the explicitRoute vector. Let's have the first one as the source, the last one as the destination.
	explicitRouteGood.push_back(nodeVector.at(0));
	explicitRouteGood.push_back(nodeVector.at(1));
	explicitRouteGood.push_back(nodeVector.at(2));
	explicitRouteGood.push_back(nodeVector.at(3));
	explicitRouteGood.push_back(nodeVector.at(4));

	// Create one explicit Route.
	//Route route(explicitRoute, true);
	//EXPECT_EQ(explicitRoute, route.getExplicitRoute());

	// Let's attempt to route a token through the nodes, following the explicit route.
	std::shared_ptr<Token> token(new Token(simulatorGlobals, 1, NULL, nodeVector.at(0), nodeVector.at(4)));
	// Attach route vector (NOT route object) to token's Route object and set it to record route.
	token->setExplicitRoute(explicitRouteGood);
	token->setRecordThisRoute();
	// Give the token to the source node and let it forward.
	nodeVector.at(0)->processAndForward(token);
	// Is the first element of recorded route really the source?
	EXPECT_EQ(nodeVector.at(0), token->getRecordedRoute().front());
	// Set the explicit route again, check whether the recorded route was erased.
	token->setExplicitRoute(explicitRouteGood);
	EXPECT_EQ(0, token->getRecordedRoute().size());
	// Now let's give the token to the first node and let it be forwarded until destination is reached.
	EXPECT_EQ(nodeVector.at(0), token->source);
	EXPECT_EQ(nodeVector.at(4), token->destination);
	// The first-hop-source-hop issue is fixed, rebuild test cases such that they do not fail.

	//// The first routing, so far, should still result in token at node zero, since this is the first node in route.
	//// Must modify Node routing such that this DO NOT pass. First next hop, if current hop, must be ignored.
	//EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token));
	//EXPECT_EQ(nodeVector.at(0), token->next);
	//EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token));
	//EXPECT_EQ(nodeVector.at(1), token->next);
	//EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	//EXPECT_EQ(nodeVector.at(2), token->next);
	//EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	//EXPECT_EQ(nodeVector.at(3), token->next);
	//EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	//EXPECT_EQ(nodeVector.at(4), token->next);
	//EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	//EXPECT_EQ(nodeVector.at(4), token->next);

	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token));
	EXPECT_EQ(nodeVector.at(1), token->next);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token->next);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(3), token->next);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(4), token->next);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(4), token->next);
	EXPECT_EQ(explicitRouteGood, token->getRecordedRoute()); // We must have followed the route as programmed.

	// Now let's build some very weird routing paths and see whether the routing functions deal with that (basically repeated hops).
	// Build this routing path: 0,0,1,2,2,2,2,2,3,4,4,4. Each repeated node should be actually visited only once.
	explicitRouteBad.push_back(nodeVector.at(0));
	explicitRouteBad.push_back(nodeVector.at(0));
	explicitRouteBad.push_back(nodeVector.at(0));
	explicitRouteBad.push_back(nodeVector.at(1));
	explicitRouteBad.push_back(nodeVector.at(2));
	explicitRouteBad.push_back(nodeVector.at(2));
	explicitRouteBad.push_back(nodeVector.at(2));
	explicitRouteBad.push_back(nodeVector.at(2));
	explicitRouteBad.push_back(nodeVector.at(2));
	explicitRouteBad.push_back(nodeVector.at(3));
	explicitRouteBad.push_back(nodeVector.at(4));
	explicitRouteBad.push_back(nodeVector.at(4));
	explicitRouteBad.push_back(nodeVector.at(4));
	token->setExplicitRoute(explicitRouteGood);
	token->setRecordThisRoute();
	// Reset source, destination, previous, next.
	token->source = nodeVector.at(0);
	token->destination = nodeVector.at(4);
	token->previous = NULL;
	token->next = NULL;
	// Now let's give the token to the first node and let it be forwarded until destination is reached.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token));
	EXPECT_EQ(nodeVector.at(1), token->next);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token->next);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(3), token->next);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(4), token->next);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(4), token->next);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token->next)->processAndForward(token)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(4), token->next);
	EXPECT_EQ(explicitRouteGood, token->getRecordedRoute()); // We must have followed basically the good route without repetitions, regardless of what the Bad route says.
}


/// Test Node statistics.
TEST_F(NodeTest, TokenStatistics) {
	// Build 3 nodes.
	//std::shared_ptr<Message> dummyContents(new Message("This is a dummy Token contents."));
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> explicitRouteGood;
	for (int i = 0; i < 3; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Push each node in a certain order to the explicitRoute vector. Let's have the first one as the source, the last one as the destination.
	explicitRouteGood.push_back(nodeVector.at(0));
	explicitRouteGood.push_back(nodeVector.at(1));
	explicitRouteGood.push_back(nodeVector.at(2));
	
	// Create token, attach the route. token created at time 0.0.
	std::shared_ptr<Token> token1(new Token(simulatorGlobals, 1, NULL, nodeVector.at(0), nodeVector.at(2)));
	// Attach route vector (NOT route object) to token's Route object and set it to record route.
	token1->setExplicitRoute(explicitRouteGood);
	token1->setRecordThisRoute();
	// Give the token to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token1));
	EXPECT_EQ(nodeVector.at(1), token1->next);
	// Advance the clock such that the token arrives at node 1 at time 15.0.
	simulatorGlobals.setCurrentAbsoluteTime(15.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token1->next)->processAndForward(token1)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token1->next);
	// Advance the clock such that the token arrives at node 2 at time 25.0.
	simulatorGlobals.setCurrentAbsoluteTime(25.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token1->next)->processAndForward(token1)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token1->next); // Token has reached destination.
	EXPECT_EQ(explicitRouteGood, token1->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 29 and attempt to have node 2 process the token. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(29.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token1->next)->processAndForward(token1)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token1->next); // Token has already reached destination.
	EXPECT_EQ(explicitRouteGood, token1->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(1, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(1, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	// Let's check some statistics for node 1.
	EXPECT_EQ(1, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(1, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(15, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_EQ(15, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(15, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getSumPduOrTokenJitter());
	// Let's check some statistics for node 2.
	EXPECT_EQ(1, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(25, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenJitter());
	// Kill token1.
	token1.reset();

	// Route two more tokens through the same path, at different times, and test statistics.
	// Create token, attach the route. token created at time 30.0.
	simulatorGlobals.setCurrentAbsoluteTime(30.0);
	std::shared_ptr<Token> token2(new Token(simulatorGlobals, 2, NULL, nodeVector.at(0), nodeVector.at(2)));
	// Attach route vector (NOT route object) to token's Route object and set it to record route.
	token2->setExplicitRoute(explicitRouteGood);
	token2->setRecordThisRoute();
	// Give the token to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token2));
	EXPECT_EQ(nodeVector.at(1), token2->next);
	// Advance the clock such that the token arrives at node 1 at time 40.0.
	simulatorGlobals.setCurrentAbsoluteTime(40.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token2->next)->processAndForward(token2)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token2->next);
	// Advance the clock such that the token arrives at node 2 at time 50.0.
	simulatorGlobals.setCurrentAbsoluteTime(50.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token2->next)->processAndForward(token2)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token2->next); // Token has reached destination.
	EXPECT_EQ(explicitRouteGood, token2->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 54 and attempt to have node 2 process the token. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(54.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token2->next)->processAndForward(token2)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token2->next); // Token has already reached destination.
	EXPECT_EQ(explicitRouteGood, token2->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(2, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	// Let's check some statistics for node 1.
	EXPECT_EQ(2, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(10, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(12.5, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getSumPduOrTokenJitter());
	// Let's check some statistics for node 2.
	EXPECT_EQ(2, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(20, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(22.5, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(45, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(2)->getSumPduOrTokenJitter());
	// Kill token2.
	token2.reset();

	// Create token, attach the route. token created at time 55.0.
	simulatorGlobals.setCurrentAbsoluteTime(55.0);
	std::shared_ptr<Token> token3(new Token(simulatorGlobals, 3, NULL, nodeVector.at(0), nodeVector.at(2)));
	// Attach route vector (NOT route object) to token's Route object and set it to record route.
	token3->setExplicitRoute(explicitRouteGood);
	token3->setRecordThisRoute();
	// Give the token to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(token3));
	EXPECT_EQ(nodeVector.at(1), token3->next);
	// Advance the clock such that the token arrives at node 1 at time 60.0.
	simulatorGlobals.setCurrentAbsoluteTime(60.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(token3->next)->processAndForward(token3)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token3->next);
	// Advance the clock such that the token arrives at node 2 at time 68.0.
	simulatorGlobals.setCurrentAbsoluteTime(68.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token3->next)->processAndForward(token3)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token3->next); // Token has reached destination.
	EXPECT_EQ(explicitRouteGood, token3->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 54 and attempt to have node 2 process the token. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(100.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(token3->next)->processAndForward(token3)); // Use the next hop to process the token, follow the path.
	EXPECT_EQ(nodeVector.at(2), token3->next); // Token has already reached destination.
	EXPECT_EQ(explicitRouteGood, token3->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(3, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(3, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	// Let's check some statistics for node 1.
	EXPECT_EQ(3, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(3, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(5, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_EQ(10, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(30, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(-10, nodeVector.at(1)->getSumPduOrTokenJitter());
	// Let's check some statistics for node 2.
	EXPECT_EQ(3, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(13, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(-7, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(19.333333333333332, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-6, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(58, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(-12, nodeVector.at(2)->getSumPduOrTokenJitter());
	// Kill token3.
	token3.reset();
}

/// Test Node statistics for PDUs.
TEST_F(NodeTest, PDUStatistics) {
	// Build 3 nodes.
	//std::shared_ptr<Message> dummyContents(new Message("This is a dummy ProtocolDataUnit contents."));
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> explicitRouteGood;
	for (int i = 0; i < 3; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Push each node in a certain order to the explicitRoute vector. Let's have the first one as the source, the last one as the destination.
	explicitRouteGood.push_back(nodeVector.at(0));
	explicitRouteGood.push_back(nodeVector.at(1));
	explicitRouteGood.push_back(nodeVector.at(2));
	
	// Create pdu, attach the route. pdu created at time 0.0. All PDUs will have size 1000 bytes.
	std::shared_ptr<ProtocolDataUnit> pdu1(new ProtocolDataUnit(simulatorGlobals, 1, NULL, nodeVector.at(0), nodeVector.at(2), 1000));
	// Get initial ttl
	unsigned short int ttl = pdu1->getTtl();
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu1->setExplicitRoute(explicitRouteGood);
	pdu1->setRecordThisRoute();
	// Give the pdu to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(pdu1));
	EXPECT_EQ(nodeVector.at(1), pdu1->next);
	EXPECT_EQ(ttl - 1, pdu1->getTtl());
	// Advance the clock such that the pdu arrives at node 1 at time 15.0.
	simulatorGlobals.setCurrentAbsoluteTime(15.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pdu1->next)->processAndForward(pdu1)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu1->next);
	EXPECT_EQ(ttl - 2, pdu1->getTtl());
	// Advance the clock such that the pdu arrives at node 2 at time 25.0.
	simulatorGlobals.setCurrentAbsoluteTime(25.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pdu1->next)->processAndForward(pdu1)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu1->next); // PDU has reached destination.
	EXPECT_EQ(ttl - 2, pdu1->getTtl());  // No forwarding, no TTL decrement.
	EXPECT_EQ(explicitRouteGood, pdu1->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 29 and attempt to have node 2 process the pdu. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(29.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pdu1->next)->processAndForward(pdu1)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu1->next); // PDU has already reached destination.
	EXPECT_EQ(ttl - 2, pdu1->getTtl());  // No forwarding, no TTL decrement.
	EXPECT_EQ(explicitRouteGood, pdu1->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(1, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(1, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	EXPECT_EQ(1000, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(1000, nodeVector.at(0)->getForwardedBytesCount());
	// Let's check some statistics for node 1.
	EXPECT_EQ(1, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(1, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(15, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_EQ(15, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(15, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getSumPduOrTokenJitter());
	EXPECT_EQ(1000, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(1000, nodeVector.at(1)->getForwardedBytesCount());
	// Let's check some statistics for node 2.
	EXPECT_EQ(1, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(25, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenJitter());
	EXPECT_EQ(1000, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	// Kill pdu1.
	pdu1.reset();

	// Route two more pdus through the same path, at different times, and test statistics.
	// Create pdu, attach the route. pdu created at time 30.0.
	simulatorGlobals.setCurrentAbsoluteTime(30.0);
	std::shared_ptr<ProtocolDataUnit> pdu2(new ProtocolDataUnit(simulatorGlobals, 2, NULL, nodeVector.at(0), nodeVector.at(2), 1000));
	// Get initial TTL.
	ttl = pdu2->getTtl();
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu2->setExplicitRoute(explicitRouteGood);
	pdu2->setRecordThisRoute();
	// Give the pdu to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(pdu2));
	EXPECT_EQ(nodeVector.at(1), pdu2->next);
	EXPECT_EQ(ttl - 1, pdu2->getTtl());
	// Advance the clock such that the pdu arrives at node 1 at time 40.0.
	simulatorGlobals.setCurrentAbsoluteTime(40.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pdu2->next)->processAndForward(pdu2)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu2->next);
	EXPECT_EQ(ttl - 2, pdu2->getTtl());
	// Advance the clock such that the pdu arrives at node 2 at time 50.0.
	simulatorGlobals.setCurrentAbsoluteTime(50.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pdu2->next)->processAndForward(pdu2)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu2->next); // PDU has reached destination.
	EXPECT_EQ(ttl - 2, pdu2->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu2->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 54 and attempt to have node 2 process the pdu. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(54.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pdu2->next)->processAndForward(pdu2)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu2->next); // PDU has already reached destination.
	EXPECT_EQ(ttl - 2, pdu2->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu2->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(2, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	EXPECT_EQ(2000, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(2000, nodeVector.at(0)->getForwardedBytesCount());
	// Let's check some statistics for node 1.
	EXPECT_EQ(2, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(10, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(12.5, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getSumPduOrTokenJitter());
	EXPECT_EQ(2000, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(2000, nodeVector.at(1)->getForwardedBytesCount());
	// Let's check some statistics for node 2.
	EXPECT_EQ(2, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(20, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(22.5, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(45, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(2)->getSumPduOrTokenJitter());
	EXPECT_EQ(2000, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	// Kill pdu2.
	pdu2.reset();

	// Create pdu, attach the route. pdu created at time 55.0.
	simulatorGlobals.setCurrentAbsoluteTime(55.0);
	std::shared_ptr<ProtocolDataUnit> pdu3(new ProtocolDataUnit(simulatorGlobals, 3, NULL, nodeVector.at(0), nodeVector.at(2), 1000));
	// Get initial TTL.
	ttl = pdu3->getTtl();
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu3->setExplicitRoute(explicitRouteGood);
	pdu3->setRecordThisRoute();
	// Give the pdu to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(pdu3));
	EXPECT_EQ(nodeVector.at(1), pdu3->next);
	EXPECT_EQ(ttl - 1, pdu3->getTtl());
	// Advance the clock such that the pdu arrives at node 1 at time 60.0.
	simulatorGlobals.setCurrentAbsoluteTime(60.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pdu3->next)->processAndForward(pdu3)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu3->next);
	EXPECT_EQ(ttl - 2, pdu3->getTtl());
	// Advance the clock such that the pdu arrives at node 2 at time 68.0.
	simulatorGlobals.setCurrentAbsoluteTime(68.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pdu3->next)->processAndForward(pdu3)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu3->next); // PDU has reached destination.
	EXPECT_EQ(ttl - 2, pdu3->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu3->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 54 and attempt to have node 2 process the pdu. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(100.0);
	EXPECT_EQ(NodeReturnType::FINAL_DESTINATION, std::dynamic_pointer_cast<Node>(pdu3->next)->processAndForward(pdu3)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu3->next); // PDU has already reached destination.
	EXPECT_EQ(ttl - 2, pdu3->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu3->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(3, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(3, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	EXPECT_EQ(3000, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(3000, nodeVector.at(0)->getForwardedBytesCount());
	// Let's check some statistics for node 1.
	EXPECT_EQ(3, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(3, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(5, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_EQ(10, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(30, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(-10, nodeVector.at(1)->getSumPduOrTokenJitter());
	EXPECT_EQ(3000, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(3000, nodeVector.at(1)->getForwardedBytesCount());
	// Let's check some statistics for node 2.
	EXPECT_EQ(3, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(13, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(-7, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(19.333333333333332, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-6, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(58, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(-12, nodeVector.at(2)->getSumPduOrTokenJitter());
	EXPECT_EQ(3000, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	// Kill pdu3.
	pdu3.reset();
}

/// Test Node statistics for PDUs, but PDU will be dropped at destination node before processing due to exceeded TTL.
TEST_F(NodeTest, PDUStatisticsExceededTTL) {
	// Build 3 nodes.
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> explicitRouteGood;
	for (int i = 0; i < 3; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Push each node in a certain order to the explicitRoute vector. Let's have the first one as the source, the last one as the destination.
	explicitRouteGood.push_back(nodeVector.at(0));
	explicitRouteGood.push_back(nodeVector.at(1));
	explicitRouteGood.push_back(nodeVector.at(2));
	
	// Create pdu, attach the route. pdu created at time 0.0. All PDUs will have size 1000 bytes.
	std::shared_ptr<ProtocolDataUnit> pdu1(new ProtocolDataUnit(simulatorGlobals, 1, NULL, nodeVector.at(0), nodeVector.at(2), 1000));
	// Set initial ttl. Only 2 hops.
	pdu1->setTtl(2);
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu1->setExplicitRoute(explicitRouteGood);
	pdu1->setRecordThisRoute();
	// Give the pdu to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(pdu1));
	EXPECT_EQ(nodeVector.at(1), pdu1->next);
	EXPECT_EQ(1, pdu1->getTtl());
	// Advance the clock such that the pdu arrives at node 1 at time 15.0.
	simulatorGlobals.setCurrentAbsoluteTime(15.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pdu1->next)->processAndForward(pdu1)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu1->next);
	EXPECT_EQ(0, pdu1->getTtl());
	// Advance the clock such that the pdu arrives at node 2 at time 25.0.
	simulatorGlobals.setCurrentAbsoluteTime(25.0);
	// TTL has exceeded. Node 2, destination, should discard it before doing any statistics processing.
	EXPECT_EQ(NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED, std::dynamic_pointer_cast<Node>(pdu1->next)->processAndForward(pdu1)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu1->next); // PDU has reached destination.
	EXPECT_EQ(0, pdu1->getTtl());  // No forwarding, no TTL decrement.
	EXPECT_EQ(explicitRouteGood, pdu1->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 29 and attempt to have node 2 process the pdu. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(29.0);
	EXPECT_EQ(NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED, std::dynamic_pointer_cast<Node>(pdu1->next)->processAndForward(pdu1)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu1->next); // PDU has already reached destination.
	EXPECT_EQ(0, pdu1->getTtl());  // No forwarding, no TTL decrement.
	EXPECT_EQ(explicitRouteGood, pdu1->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(1, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(1, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	EXPECT_EQ(1000, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(1000, nodeVector.at(0)->getForwardedBytesCount());
	// Let's check some statistics for node 1.
	EXPECT_EQ(1, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(1, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(15, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_EQ(15, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(15, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(1)->getSumPduOrTokenJitter());
	EXPECT_EQ(1000, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(1000, nodeVector.at(1)->getForwardedBytesCount());
	// Let's check some statistics for node 2.
	EXPECT_EQ(0, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	// Kill pdu1.
	pdu1.reset();

	// Route two more pdus through the same path, at different times, and test statistics.
	// Create pdu, attach the route. pdu created at time 30.0.
	simulatorGlobals.setCurrentAbsoluteTime(30.0);
	std::shared_ptr<ProtocolDataUnit> pdu2(new ProtocolDataUnit(simulatorGlobals, 2, NULL, nodeVector.at(0), nodeVector.at(2), 1000));
	// Set initial ttl. Only 2 hops.
	pdu2->setTtl(2);
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu2->setExplicitRoute(explicitRouteGood);
	pdu2->setRecordThisRoute();
	// Give the pdu to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(pdu2));
	EXPECT_EQ(nodeVector.at(1), pdu2->next);
	EXPECT_EQ(1, pdu2->getTtl());
	// Advance the clock such that the pdu arrives at node 1 at time 40.0.
	simulatorGlobals.setCurrentAbsoluteTime(40.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pdu2->next)->processAndForward(pdu2)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu2->next);
	EXPECT_EQ(0, pdu2->getTtl());
	// Advance the clock such that the pdu arrives at node 2 at time 50.0.
	simulatorGlobals.setCurrentAbsoluteTime(50.0);
	EXPECT_EQ(NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED, std::dynamic_pointer_cast<Node>(pdu2->next)->processAndForward(pdu2)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu2->next); // PDU has reached destination.
	EXPECT_EQ(0, pdu2->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu2->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 54 and attempt to have node 2 process the pdu. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(54.0);
	EXPECT_EQ(NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED, std::dynamic_pointer_cast<Node>(pdu2->next)->processAndForward(pdu2)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu2->next); // PDU has already reached destination.
	EXPECT_EQ(0, pdu2->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu2->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(2, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	EXPECT_EQ(2000, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(2000, nodeVector.at(0)->getForwardedBytesCount());
	// Let's check some statistics for node 1.
	EXPECT_EQ(2, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(2, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(10, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(12.5, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(25, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getSumPduOrTokenJitter());
	EXPECT_EQ(2000, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(2000, nodeVector.at(1)->getForwardedBytesCount());
	// Let's check some statistics for node 2.
	EXPECT_EQ(0, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(4, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(0, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	// Kill pdu2.
	pdu2.reset();

	// Create pdu, attach the route. pdu created at time 55.0.
	simulatorGlobals.setCurrentAbsoluteTime(55.0);
	std::shared_ptr<ProtocolDataUnit> pdu3(new ProtocolDataUnit(simulatorGlobals, 3, NULL, nodeVector.at(0), nodeVector.at(2), 1000));
	// Set initial ttl. Only 2 hops.
	pdu3->setTtl(2);
	// Attach route vector (NOT route object) to pdu's Route object and set it to record route.
	pdu3->setExplicitRoute(explicitRouteGood);
	pdu3->setRecordThisRoute();
	// Give the pdu to the source node and let it forward to the destination. But, before each hop, forward the simulation clock.
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, nodeVector.at(0)->processAndForward(pdu3));
	EXPECT_EQ(nodeVector.at(1), pdu3->next);
	EXPECT_EQ(1, pdu3->getTtl());
	// Advance the clock such that the pdu arrives at node 1 at time 60.0.
	simulatorGlobals.setCurrentAbsoluteTime(60.0);
	EXPECT_EQ(NodeReturnType::PDU_ROUTE_UPDATED, std::dynamic_pointer_cast<Node>(pdu3->next)->processAndForward(pdu3)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu3->next);
	EXPECT_EQ(0, pdu3->getTtl());
	// Advance the clock such that the pdu arrives at node 2 at time 68.0.
	simulatorGlobals.setCurrentAbsoluteTime(68.0);
	EXPECT_EQ(NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED, std::dynamic_pointer_cast<Node>(pdu3->next)->processAndForward(pdu3)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu3->next); // PDU has reached destination.
	EXPECT_EQ(0, pdu3->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu3->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's advance the clock again to 54 and attempt to have node 2 process the pdu. The statistics should *NOT* be updated for this "arrival."
	simulatorGlobals.setCurrentAbsoluteTime(100.0);
	EXPECT_EQ(NodeReturnType::TTL_EXCEEDED_PDU_DISCARDED, std::dynamic_pointer_cast<Node>(pdu3->next)->processAndForward(pdu3)); // Use the next hop to process the pdu, follow the path.
	EXPECT_EQ(nodeVector.at(2), pdu3->next); // PDU has already reached destination.
	EXPECT_EQ(0, pdu3->getTtl());
	EXPECT_EQ(explicitRouteGood, pdu3->getRecordedRoute()); // We must have followed the route as programmed.
	// Let's check some statistics for node 0.
	EXPECT_EQ(3, nodeVector.at(0)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(3, nodeVector.at(0)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getLastPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(0)->getSumPduOrTokenJitter());
	EXPECT_EQ(3000, nodeVector.at(0)->getReceivedBytesCount());
	EXPECT_EQ(3000, nodeVector.at(0)->getForwardedBytesCount());
	// Let's check some statistics for node 1.
	EXPECT_EQ(3, nodeVector.at(1)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(3, nodeVector.at(1)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(1)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(5, nodeVector.at(1)->getLastPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getLastPduOrTokenJitter());
	EXPECT_EQ(10, nodeVector.at(1)->getMeanPduOrTokenDelay());
	EXPECT_EQ(-5, nodeVector.at(1)->getMeanPduOrTokenJitter());
	EXPECT_EQ(30, nodeVector.at(1)->getSumPduOrTokenDelay());
	EXPECT_EQ(-10, nodeVector.at(1)->getSumPduOrTokenJitter());
	EXPECT_EQ(3000, nodeVector.at(1)->getReceivedBytesCount());
	EXPECT_EQ(3000, nodeVector.at(1)->getForwardedBytesCount());
	// Let's check some statistics for node 2.
	EXPECT_EQ(0, nodeVector.at(2)->getReceivedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedPdusOrTokensCount());
	EXPECT_EQ(6, nodeVector.at(2)->getDroppedPdusOrTokensCount());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getLastPduOrTokenJitter());
	EXPECT_DOUBLE_EQ(0, nodeVector.at(2)->getMeanPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getMeanPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenDelay());
	EXPECT_EQ(0, nodeVector.at(2)->getSumPduOrTokenJitter());
	EXPECT_EQ(0, nodeVector.at(2)->getReceivedBytesCount());
	EXPECT_EQ(0, nodeVector.at(2)->getForwardedBytesCount());
	// Kill pdu3.
	pdu3.reset();
}