/**
 * @author Marcos Portnoi
 * @date June 2013
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

// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:


// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include "TokenTest.h"

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
//
// <TechnicalDetails>
//
// In Google Test, tests are grouped into test cases.  This is how we
// keep test code organized.  You should put logically related tests
// into the same test case.
//
// The test case name and the test name should both be valid C++
// identifiers.  And you should not use underscore (_) in the names.
//
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.
//
// </TechnicalDetails>

/**
 * Constructor.
 *
 * Do initializations here.
 */
TokenTest::TokenTest(): message(new Message("This is message 1")),
		dummyEntitySource(new Message("This is dummy Entity 1")), 
		dummyEntityDestination(new Message("This is dummy Entity 2")),
		newDummyEntitySource(new Message("This is dummy Entity 1")), 
		newDummyEntityDestination(new Message("This is dummy Entity 2")),
		newMessage(new Message("This is a new message")),
		simulatorGlobals(SimulatorGlobals(0.0, 0.0, false, "TokenTest")) {
}

/// Tests Token class, constructor with pointers to Entities.
TEST_F(TokenTest, ConstructorPointers) {
	std::shared_ptr<Token> token(new Token(0, 0, message, dummyEntitySource, dummyEntityDestination));

	EXPECT_EQ(0, token->id);
	EXPECT_EQ(0, token->priority);
	EXPECT_EQ(dummyEntitySource, token->source);
	EXPECT_EQ(dummyEntityDestination, token->destination);
}

/// Tests Token class, changing value of members.
TEST_F(TokenTest, ChangeValues) {
	// Create token.
	std::shared_ptr<Token> token(new Token(0, 0, message, dummyEntitySource, dummyEntityDestination));
	// Change member values.
	token->id = 10;
	token->priority++;
	EXPECT_EQ(10, token->id);
	EXPECT_EQ(1, token->priority);
	
	// Change pointer members.
	token->source = newDummyEntitySource;
	token->destination = newDummyEntityDestination;
	token->associatedEntity = newMessage;
	EXPECT_EQ(newDummyEntitySource, token->source);
	EXPECT_EQ(newDummyEntityDestination, token->destination);
	EXPECT_EQ(newMessage, token->associatedEntity);

	// Now change the contents of an Entity inside the token, without changing the address (pointer).
	newMessage->setContents("I am now altering the contents of newMessage");
	EXPECT_EQ(newMessage, token->associatedEntity);

	// Now modify the contents of an Entity by accessing the pointer inside Token.
	std::dynamic_pointer_cast<Message>(token->associatedEntity)->setContents("I am now modified through pointer.");
	EXPECT_EQ("I am now modified through pointer.", std::dynamic_pointer_cast<Message>(token->associatedEntity)->getContents());
}

/// Tests Token class, members pointing to local variables.
/// Cannot do this. It does not make sense: since smart pointers intend to control ownership, passing addresses of local variables
/// to smart pointers would disable this control, rendering the concept of smart pointers useless. Wisely, the compiler denies passing an
/// address to a smart pointer parameter.

/// Tests on Token as local variable.
TEST_F(TokenTest, TokenAsLocalVariable) {
	// Create token as local variable.
	Token token = Token(0, 0, message, dummyEntitySource, dummyEntityDestination);
	// Change member values.
	token.id = 10;
	token.priority++;
	EXPECT_EQ(10, token.id);
	EXPECT_EQ(1, token.priority);
	
	// Change pointer members.
	token.source = newDummyEntitySource;
	token.destination = newDummyEntityDestination;
	token.associatedEntity = newMessage;
	EXPECT_EQ(newDummyEntitySource, token.source);
	EXPECT_EQ(newDummyEntityDestination, token.destination);
	EXPECT_EQ(newMessage, token.associatedEntity);

	// Now change the contents of an Entity inside the token, without changing the address (pointer).
	newMessage->setContents("I am now altering the contents of newMessage");
	EXPECT_EQ(newMessage, token.associatedEntity);

	// Now modify the contents of an Entity by accessing the pointer inside Token.
	std::dynamic_pointer_cast<Message>(token.associatedEntity)->setContents("I am now modified through pointer.");
	EXPECT_EQ("I am now modified through pointer.", std::dynamic_pointer_cast<Message>(token.associatedEntity)->getContents());
}

/// Tests on members inside token made nullptr.
TEST_F(TokenTest, NullValues) {
	// Create token as local variable.
	Token token = Token(0, 0, message, dummyEntitySource, dummyEntityDestination);
	
	// Verify the count of shared pointers. Should be 2, since they exist both in this object, and inside Token object.
	EXPECT_EQ(2, token.source.use_count());
	EXPECT_EQ(2, token.destination.use_count());
	EXPECT_EQ(2, token.associatedEntity.use_count());
	
	// Now delete the local shared pointers.
	message.reset();
	dummyEntitySource.reset();
	dummyEntityDestination.reset();
	
	// Verify the count of shared pointers. Should be 1, since the local ones were deleted, but they still exist inside Token object.
	EXPECT_EQ(1, token.source.use_count());
	EXPECT_EQ(1, token.destination.use_count());
	EXPECT_EQ(1, token.associatedEntity.use_count());
	
	// Delete the Token pointer members.
	token.source.reset();
	token.destination.reset();
	token.associatedEntity.reset();
	EXPECT_EQ(0, token.source.get());
	EXPECT_EQ(0, token.destination.get());
	EXPECT_EQ(0, token.associatedEntity.get());

	// Verify the count of shared pointers. Should be zero, since they were deleted, as well as the local ones.
	EXPECT_EQ(0, token.source.use_count());
	EXPECT_EQ(0, token.destination.use_count());
	EXPECT_EQ(0, token.associatedEntity.use_count());

	// What is unique method now?
	EXPECT_FALSE(token.source.unique());
}

/// Tests routing functions in Token.
TEST_F(TokenTest, Routing) {
	// Build 5 nodes. Will test routing between them using explicit route attached to tokens. No need for links or facilities here.
	//std::shared_ptr<Message> dummyContents(new Message("This is a dummy Token contents."));
	std::vector<std::shared_ptr<Node>> nodeVector;
	std::vector<std::shared_ptr<Entity>> explicitRoute;
	for (int i = 0; i < 5; ++i) {
		nodeVector.push_back(std::shared_ptr<Node>(new Node(simulatorGlobals)));
	}
	// Push each node in a certain order to the explicitRoute vector. Let's have the first one as the source, the last one as the destination.
	explicitRoute.push_back(nodeVector.at(0));
	explicitRoute.push_back(nodeVector.at(1));
	explicitRoute.push_back(nodeVector.at(2));
	explicitRoute.push_back(nodeVector.at(3));
	explicitRoute.push_back(nodeVector.at(4));

	// Create one explicit Route.
	//Route route(explicitRoute, true);
	//EXPECT_EQ(explicitRoute, route.getExplicitRoute());

	// Let's test the basic functions for setting and getting explicit route.
	std::shared_ptr<Token> token(new Token(simulatorGlobals, 1, nullptr, nodeVector.at(0), nodeVector.at(4)));
	EXPECT_EQ(token->source, nodeVector.at(0));
	EXPECT_EQ(token->destination, nodeVector.at(4));
	EXPECT_FALSE(token->previous); // That's the way to test for nullptr smart pointers.
	EXPECT_FALSE(token->next);
	// Attach route vector (NOT route object) to token's Route object and set it to record route.
	token->setExplicitRoute(explicitRoute);
	token->setRecordThisRoute();
	EXPECT_EQ(explicitRoute, token->getExplicitRoute());
	// Give the token to the source node and let it forward.
	nodeVector.at(0)->processAndForward(token);
	// Is the first element of recorded route really the source?
	EXPECT_EQ(nodeVector.at(0), token->getRecordedRoute().front());
	// Set the explicit route again, check whether the recorded route was erased.
	token->setExplicitRoute(explicitRoute);
	EXPECT_EQ(0, token->getRecordedRoute().size());
	// Se the record route to false; route should not be recorded.
	token->setDoNotRecordThisRoute();
	// Let's push hops into the recordedRoute.
	token->addHopToRecordedRoute(nodeVector.at(0));
	token->addHopToRecordedRoute(nodeVector.at(1));
	token->addHopToRecordedRoute(nodeVector.at(2));
	token->addHopToRecordedRoute(nodeVector.at(3));
	token->addHopToRecordedRoute(nodeVector.at(4));
	// Recorded route should be empty.
	EXPECT_TRUE(token->getRecordedRoute().empty());
	// Now enable route recording and repeat.
	token->setRecordThisRoute();
	// Let's push hops into the recordedRoute.
	token->addHopToRecordedRoute(nodeVector.at(0));
	token->addHopToRecordedRoute(nodeVector.at(1));
	token->addHopToRecordedRoute(nodeVector.at(2));
	token->addHopToRecordedRoute(nodeVector.at(3));
	token->addHopToRecordedRoute(nodeVector.at(4));
	// That was basically explicitRoute vector. Verify.
	EXPECT_EQ(explicitRoute, token->getRecordedRoute());
	// Now manually furnish nodes to token and allow it to update its previous/next members by calling the forward function.
	// Route is Node 0, 1, 2, 3, 4. Source is 0, destination is 4.
	// Begin with source node. Since the next hop in the list is node 0, previous will be zero, next will be zero. The update function within Token does not treat this.
	token->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), token->previous);
	EXPECT_EQ(nodeVector.at(0), token->next); 
	// Observe that while the token is still "at" current node 0, the next one will be 1 and the previous is 0 after updating forward members. Token is "about" to exit node 0.
	// When the token reaches node 1, the current node is 1, the previous node is 0, and the next node is 1 (before updating forward members).
	// Now "forward" to node 1, the next hop.
	token->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), token->previous);
	EXPECT_EQ(nodeVector.at(1), token->next);
	// Now "forward" to node 2, the next hop.
	token->updateHopsFromExplicitRoute(nodeVector.at(1));
	EXPECT_EQ(nodeVector.at(1), token->previous);
	EXPECT_EQ(nodeVector.at(2), token->next);
	// Now "forward" to node 3, the next hop.
	token->updateHopsFromExplicitRoute(nodeVector.at(2));
	EXPECT_EQ(nodeVector.at(2), token->previous);
	EXPECT_EQ(nodeVector.at(3), token->next);
	// Now "forward" to node 4, the next hop.
	token->updateHopsFromExplicitRoute(nodeVector.at(3));
	EXPECT_EQ(nodeVector.at(3), token->previous);
	EXPECT_EQ(nodeVector.at(4), token->next);
	// Now attempt forwarding past node 4. The token should get stuck at node 4, since the token has no intelligence to consider itself reaching a destination. Attempt this three times.
	token->updateHopsFromExplicitRoute(nodeVector.at(4));
	EXPECT_EQ(nodeVector.at(4), token->previous);
	EXPECT_EQ(nodeVector.at(4), token->next);
	token->updateHopsFromExplicitRoute(nodeVector.at(4));
	EXPECT_EQ(nodeVector.at(4), token->previous);
	EXPECT_EQ(nodeVector.at(4), token->next);
	token->updateHopsFromExplicitRoute(nodeVector.at(4));
	EXPECT_EQ(nodeVector.at(4), token->previous);
	EXPECT_EQ(nodeVector.at(4), token->next);
	
	// Let's attach no route.
	token->setExplicitRoute(std::vector<std::shared_ptr<Entity>>());
	EXPECT_TRUE(token->getExplicitRoute().empty());
	EXPECT_TRUE(token->getRecordedRoute().empty());
	// Now if we attempt to have a node forward the token, it should not work.
	token->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), token->previous);
	EXPECT_FALSE(token->next); // This should be null.
	// Try again. Same outcome.
	token->updateHopsFromExplicitRoute(nodeVector.at(0));
	EXPECT_EQ(nodeVector.at(0), token->previous);
	EXPECT_FALSE(token->next); // This should be null.
	// Have another node forward the token. Only the previous field should be modified.
	token->updateHopsFromExplicitRoute(nodeVector.at(3));
	EXPECT_EQ(nodeVector.at(3), token->previous);
	EXPECT_FALSE(token->next); // This should be null.

	
}
