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
#include "../QcnSim/Event.h"
#include "../QcnSim/Message.h"
#include "../QcnSim/Scheduler.h"
#include "../QcnSim/Token.h"

#include <iostream>


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


// Tests Message class and methods.
TEST(MessageTest, SetAndGetContents) {
	Message message1;
	Message message2;
	std::string contents1("This is contents 1");
	std::string contents2("This is contents 2");
	message1.setContents(std::string("This is contents 1"));
	message2.setContents(std::string("This is contents 2"));

	EXPECT_EQ(contents1, message1.getContents());
	EXPECT_EQ(contents2, message2.getContents());
  
}

// Tests Tokene class.
TEST(TokenTest, Construction) {
	std::shared_ptr<Entity> entity = NULL;
	Token token = Token(0, 0, entity);

	EXPECT_EQ(contents1, message1.getContents());
	EXPECT_EQ(contents2, message2.getContents());
  
}

//// Tests factorial of 0.
//TEST(FactorialTest, Zero) {
//  EXPECT_EQ(1, Factorial(0));
//}
//
//// Tests factorial of positive numbers.
//TEST(FactorialTest, Positive) {
//  EXPECT_EQ(1, Factorial(1));
//  EXPECT_EQ(2, Factorial(2));
//  EXPECT_EQ(6, Factorial(3));
//  EXPECT_EQ(40320, Factorial(8));
//}
//
//
//// Tests IsPrime()``
//
//// Tests negative input.
//TEST(IsPrimeTest, Negative) {
//  // This test belongs to the IsPrimeTest test case.
//
//  EXPECT_FALSE(IsPrime(-1));
//  EXPECT_FALSE(IsPrime(-2));
//  EXPECT_FALSE(IsPrime(INT_MIN));
//}
//
//// Tests some trivial cases.
//TEST(IsPrimeTest, Trivial) {
//  EXPECT_FALSE(IsPrime(0));
//  EXPECT_FALSE(IsPrime(1));
//  EXPECT_TRUE(IsPrime(2));
//  EXPECT_TRUE(IsPrime(3));
//}
//
//// Tests positive input.
//TEST(IsPrimeTest, Positive) {
//  EXPECT_FALSE(IsPrime(4));
//  EXPECT_TRUE(IsPrime(5));
//  EXPECT_FALSE(IsPrime(6));
//  EXPECT_TRUE(IsPrime(23));
//}
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
