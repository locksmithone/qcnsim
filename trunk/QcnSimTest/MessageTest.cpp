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

#include "../gtest/include/gtest/gtest.h"
#include "../QcnSim/Message.h"

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


/// Tests Message class, constructor with parameter, get method.
TEST(MessageTest, GetContents) {
	Message message1 = Message("This is contents 1");
	Message message2 = Message("This is contents 2");
	
	EXPECT_EQ("This is contents 1", message1.getContents());
	EXPECT_EQ("This is contents 2", message2.getContents());
  
}

/// Tests Message class, constructor with parameter and variable
TEST(MessageTest, ConstructorAndVariable) {
	std::string contents1("This is contents 1");
	std::string contents2("This is contents 2");
	Message message1 = Message(contents1);
	Message message2 = Message(contents2);
	
	EXPECT_EQ(contents1, message1.getContents());
	EXPECT_EQ(contents2, message2.getContents());
  
}

/// Tests Message class and set method.
TEST(MessageTest, SetContents) {
	Message message1;
	Message message2;
	std::string contents1("This is contents 1");
	std::string contents2("This is contents 2");
	message1.setContents(std::string("This is contents 1"));
	message2.setContents(std::string("This is contents 2"));

	EXPECT_EQ(contents1, message1.getContents());
	EXPECT_EQ(contents2, message2.getContents());
  
}

/// Tests Message class, constructor with parameter.
/// Verify whether object is independent from variable used for creation.
TEST(MessageTest, ObjectIndependence) {
	Message message1;
	Message message2;
	std::string contents1("This is contents 1");
	std::string contents2("This is contents 2");
	message1.setContents(contents1);
	message2.setContents(contents2);

	EXPECT_EQ(contents1, message1.getContents());
	EXPECT_EQ(contents2, message2.getContents());
  
	// Now change the variables, see whether objects remain the same.

	contents1 = "This is new, altered contents 1.";
	contents2 = "This is new, altered contents 2.";
  
	EXPECT_EQ("This is contents 1", message1.getContents());
	EXPECT_EQ("This is contents 2", message2.getContents());
}




//GTEST_API_ int main(int argc, char **argv) {
//	std::cout << "Running main() from sample1_unittest.cc\n" << std::endl;
//	testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//}
