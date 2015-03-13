/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include "TestFixtureBase.hpp"

class FlowControlTests : public TestFixtureBase
{
public:
	static void TestSingleIf()
	{
		TestScript("\n\
			var result = false; \n\
			if(true){ \n\
				result = true; \n\
			} \n\
			\n\
			AssertEquals(true, result);\n\
		");
	}

	static void TestIfElse()
	{
		TestScript("\n\
			var result = false; \n\
			if(false){ \n\
				result = false; \n\
			} else { \n\
				result = true; \n\
			}\n\
			\n\
			AssertEquals(true, result);\n\
		");
	}

	static void TestIfElseIf()
	{	
		TestScript("\n\
			var result = false; \n\
			if(false){ \n\
				result = false; \n\
			} else if(true) { \n\
				result = true; \n\
			}\n\
			\n\
			AssertEquals(true, result);\n\
		");
	}

	static void TestElseIfChain()
	{
		TestScript("\n\
			var result = false; \n\
			if(false){ \n\
				result = false; \n\
			} else if(false) { \n\
				result = false; \n\
			} else { \n\
				result = true; \n\
			} \n\
			\n\
			AssertEquals(true, result);\n\
		");
	}

	static void TestWhile()
	{
		TestScript("\n\
			var count = 0; \n\
			var i = 0; \n\
			while(i < 10){ \n\
				count = count + 1; \n\
				i = i + 1;\n\
			};\n\
			;\n\
			AssertEquals(10, count);\n\
		");

		TestScript("\n\
			var count = 0; \n\
			var i = 0; \n\
			while(i < 10){ \n\
				var j = 0; \n\
				while(j < 10){ \n\
					count = count + 1; \n\
					j = j + 1;\n\
				} \n\
				i = i + 1;\n\
			};\n\
			;\n\
			AssertEquals(100, count);\n\
		");
	}

	static void TestFor()
	{
		TestScript("\n\
			var count = 0; \n\
			for(var i = 0; i < 10; i = i + 1){ \n\
				count = count + 1; \n\
			};\n\
			;\n\
			AssertEquals(10, count);\n\
		");
	}

	static void TestNestedFor()
	{
		TestScript("\n\
			var count = 0; \n\
			for(var i = 0; i < 10; i = i + 1){ \n\
				for(var j = 0; j < 10; j = j + 1){ \n\
					count = count + 1; \n\
				} \n\
			};\n\
			;\n\
			AssertEquals(100, count);\n\
		");
	}

	static void TestSequentialFor()
	{
		TestScript("\n\
			var count = 0; \n\
			for(var i = 0; i < 10; i = i + 1){ \n\
				count = count + 1; \n\
			};\n\
			\n\
			for (var i = 0; i < 10; i = i + 1){\n\
				\n\
					count = count + 1; \n\
			}; \n\
			;\n\
			AssertEquals(20, count);\n\
		");
	}

	static void TestBreak()
	{
		TestScript("\n\
			var count = 0; \n\
			var i = 0; \n\
			while(i < 10){ \n\
				if(i == 5){ \n\
					break; \n\
				} \n\
				count = count + 1; \n\
				i = i + 1;\n\
			};\n\
			;\n\
			AssertEquals(5, count);\n\
		");
	}

	static void TestBreakFailsOutsideLoop()
	{
		TEST_SCRIPT_FAILURE(CompilerException, "break;");
	}

	static void TestContinue()
	{
		TestScript("\n\
			var count = 0; \n\
			var i = 0; \n\
			while(i < 10){ \n\
				if(i == 5){ \n\
					i = i + 1; \n\
					continue; \n\
				} \n\
				count = count + 1; \n\
				i = i + 1;\n\
			};\n\
			;\n\
			AssertEquals(9, count);\n\
		");
	}


	static void TestContinueFailsOutsideLoop()
	{
		TEST_SCRIPT_FAILURE(CompilerException, "continue;");
	}

	FlowControlTests() : TestFixtureBase("FlowControlTests")
	{
		ADD_TESTCASE(TestSingleIf);
		ADD_TESTCASE(TestIfElse);
		ADD_TESTCASE(TestIfElseIf);
		ADD_TESTCASE(TestElseIfChain);
		ADD_TESTCASE(TestWhile);
		ADD_TESTCASE(TestFor);
		ADD_TESTCASE(TestNestedFor);
		ADD_TESTCASE(TestSequentialFor);
		ADD_TESTCASE(TestBreak);
		ADD_TESTCASE(TestBreakFailsOutsideLoop);
		ADD_TESTCASE(TestContinue);
		ADD_TESTCASE(TestContinueFailsOutsideLoop);
	}
};