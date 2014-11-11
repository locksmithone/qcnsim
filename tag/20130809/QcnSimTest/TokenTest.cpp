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
		newMessage(new Message("This is a new message")) {
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

/// Tests on members inside token made NULL.
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

//
// /*Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Isn't this convenient?*/
//

//GTEST_API_ int main(int argc, char **argv) {
//	std::cout << "Running main() from sample1_unittest.cc\n" << std::endl;
//	testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//}
