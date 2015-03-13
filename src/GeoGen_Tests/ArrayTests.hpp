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

class ArrayTests : public TestFixtureBase
{
public:
	static void TestCountEmpty()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			\n\
			AssertEquals(0, a.Count());\n\
		");
	}

	static void TestCountEmptyWithLiteral()
	{
		TestScript("\n\
			var a = {};\n\
			\n\
			AssertEquals(0, a.Count());\n\
		");
	}

	static void TestPushBackIncreasesCount()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(7);\n\
			\n\
			AssertEquals(1, a.Count());\n\
		");
	}

	static void TestPushBackFront()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(2);\n\
			a.PushBack(3);\n\
			a.PushBack(1);\n\
			\n\
			AssertEquals(2, a.Front());\n\
		");
	}

	static void TestPushBackBack()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(2);\n\
			a.PushBack(3);\n\
			a.PushBack(1);\n\
			\n\
			AssertEquals(1, a.Back());\n\
		");
	}

	static void TestGetByAutoIndex()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a.PushBack(11);\n\
			a.PushBack(12);\n\
			a.PushBack(13);\n\
			\n\
			AssertEquals(11, a[0]);\n\
			AssertEquals(12, a[1]);\n\
			AssertEquals(13, a[2]);\n\
		");
	}
	
	static void TestGetByAutoWithLiteral()
	{
		TestScript("\n\
		    var a = { 11, 12, 13 };\n\
			\n\
			AssertEquals(11, a[0]);\n\
			AssertEquals(12, a[1]);\n\
			AssertEquals(13, a[2]);\n\
		");
	}

	static void TestIndexerPassThrough()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			\n\
			AssertEquals(20, a[5] = 20);\n\
		");
	}

	static void TestSetGet()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[5] = 20;\n\
			\n\
			AssertEquals(20, a[5]);\n\
		");
	}

	static void TestSetGetWithLiteral()
	{
		TestScript("\n\
			var a = { 5: 20 };\n\
			\n\
			AssertEquals(20, a[5]);\n\
		");
	}

	static void TestSetGetString()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(21, a[\"key\"]);\n\
		");
	}

	static void TestNullKeyFails()
	{
		TEST_SCRIPT_FAILURE(NullKeyException, "\n\
			var a = Array.Empty();\n\
			a[null] = 21;\n\
		");
	}

	static void TestNullKeyWithLiteralFails()
	{
		TEST_SCRIPT_FAILURE(NullKeyException, "\n\
			var a = { null: 21 };\n\
		");
	}

	static void TestContainsKeyReturnsTrue()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(true, a.ContainsKey(\"key\"));\n\
		");
	}

	static void TestContainsKeyReturnsFalse()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(false, a.ContainsKey(\"another\"));\n\
		");
	}

	static void TestContainsValueReturnsTrue()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(true, a.ContainsValue(21));\n\
		");
	}

	static void TestContainsValueReturnsFalse()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			\n\
			AssertEquals(false, a.ContainsValue(29));\n\
		");
	}

	static void TestRemoveKeyRemoves()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveKey(\"key\");\n\
			\n\
			AssertEquals(false, a.ContainsKey(\"key\"));\n\
		");
	}

	static void TestRemoveKeyThrowsIfNotFound()
	{
		TEST_SCRIPT_FAILURE(ArrayKeyNotFoundException, "\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveKey(\"key5\");\n\
		");
	}

	static void TestRemoveValueRemoves()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveValue(21);\n\
			\n\
			AssertEquals(false, a.ContainsKey(\"key\"));\n\
		");
	}

	static void TestRemoveValueRemovesOnlyFirst()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 21;\n\
			a.RemoveValue(21);\n\
			\n\
			AssertEquals(true, a.ContainsKey(\"key2\"));\n\
		");
	}

	static void TestRemoveValueDoesNothingIfNotFound()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"key\"] = 21;\n\
			a[\"key2\"] = 22;\n\
			a.RemoveValue(25);\n\
			\n\
			AssertEquals(2, a.Count());\n\
		");
	}

	static void TestIndexerPushBack()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[] = 11;\n\
			a[] = 12;\n\
			a[] = 13;\n\
			\n\
			AssertEquals(11, a[0]);\n\
			AssertEquals(12, a[1]);\n\
			AssertEquals(13, a[2]);\n\
		");
	}

	static void TestGetKeyByIndex()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"a\"] = 11;\n\
			a[\"b\"] = 12;\n\
			a[\"b\"] = 13;\n\
			\n\
			AssertEquals(\"b\", a.GetKeyByIndex(1));\n\
		");
	}

	static void TestGetKeyByIndexFailsWithNegativeIndex()
	{
		TEST_SCRIPT_FAILURE(ArrayIndexNotFoundException, "\n\
			var a = Array.Empty();\n\
			a[\"a\"] = 11;\n\
			a[\"b\"] = 12;\n\
			a[\"b\"] = 13;\n\
			\n\
			a.GetKeyByIndex(-5);\n\
		");
	}

	static void TestGetKeyByIndexFailsWithTooHighIndex()
	{
		TEST_SCRIPT_FAILURE(ArrayIndexNotFoundException, "\n\
			var a = Array.Empty();\n\
			a[\"a\"] = 11;\n\
			a[\"b\"] = 12;\n\
			a[\"b\"] = 13;\n\
			\n\
			a.GetKeyByIndex(3);\n\
		");
	}

	static void TestGetValueByIndex()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"a\"] = 11;\n\
			a[\"b\"] = 12;\n\
			a[\"c\"] = 13;\n\
			\n\
			AssertEquals(12, a.GetValueByIndex(1));\n\
		");
	}

	static void TestGetValueByIndexFailsWithNegativeIndex()
	{
		TEST_SCRIPT_FAILURE(ArrayIndexNotFoundException, "\n\
			var a = Array.Empty();\n\
			a[\"a\"] = 11;\n\
			a[\"b\"] = 12;\n\
			a[\"c\"] = 13;\n\
			\n\
			a.GetValueByIndex(-5);\n\
		");
	}

	static void TestGetValueByIndexFailsWithTooHighIndex()
	{
		TEST_SCRIPT_FAILURE(ArrayIndexNotFoundException, "\n\
			var a = Array.Empty();\n\
			a[\"a\"] = 11;\n\
			a[\"b\"] = 12;\n\
			a[\"b\"] = 13;\n\
			\n\
			a.GetValueByIndex(3);\n\
		");
	}

	static void TestSortByKeys()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"c\"] = 11;\n\
			a[\"a\"] = 12;\n\
			a[\"b\"] = 13;\n\
			\n\
			a.SortByKeys();\n\
			\n\
			AssertEquals(12, a.GetValueByIndex(0)); \n\
			AssertEquals(13, a.GetValueByIndex(1)); \n\
			AssertEquals(11, a.GetValueByIndex(2)); \n\
		");
	}

	static void TestSortByValues()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[\"c\"] = 11;\n\
			a[\"a\"] = 13;\n\
			a[\"b\"] = 12;\n\
			\n\
			a.SortByValues();\n\
			\n\
			AssertEquals(11, a.GetValueByIndex(0)); \n\
			AssertEquals(12, a.GetValueByIndex(1)); \n\
			AssertEquals(13, a.GetValueByIndex(2)); \n\
		");
	}

	static void TestSortByValuesMultipleTypes()
	{
		// The types will be ordererd alphabetically
		TestScript("\n\
			var a = Array.Empty();\n\
			a[] = \"bbb\";\n\
			a[] = {0, 2, 4};\n\
			a[] = \"aaaa\";\n\
			a[] = 12;\n\
			\n\
			a.SortByValues();\n\
			\n\
			a.GetValueByIndex(0)[1]; \n\
			AssertEquals(12, a.GetValueByIndex(1)); \n\
			AssertEquals(\"aaaa\", a.GetValueByIndex(2)); \n\
			AssertEquals(\"bbb\", a.GetValueByIndex(3)); \n\
		");
	}

	static void TestArrayReferenceWithIncrement()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			a[5] = 2;\n\
			AssertEquals(3, ++a[5]); \n\
			AssertEquals(3, a[5]); \n\
		");
	}

	static void TestArrayReferenceWithIncrementInSubscript()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			var i = 4;\n\
			a[5] = 2;\n\
			AssertEquals(2, a[++i]++); \n\
			AssertEquals(3, a[5]); \n\
		");
	}

	static void TestArrayReferenceWithDoubleIncrement()
	{
		TestScript("\n\
			var a = Array.Empty();\n\
			var i = 3;\n\
			a[4] = 1;\n\
			AssertEquals(6, a[++i] += ++i); \n\
			AssertEquals(6, a[4]); \n\
		");
	}

	ArrayTests() : TestFixtureBase("ArrayTests")
	{
		ADD_TESTCASE(TestCountEmpty);
		ADD_TESTCASE(TestCountEmptyWithLiteral);
		ADD_TESTCASE(TestPushBackIncreasesCount);
		ADD_TESTCASE(TestPushBackFront);
		ADD_TESTCASE(TestPushBackBack);
		ADD_TESTCASE(TestGetByAutoIndex);
		ADD_TESTCASE(TestGetByAutoWithLiteral);
		ADD_TESTCASE(TestIndexerPassThrough);
		ADD_TESTCASE(TestSetGet);
		ADD_TESTCASE(TestSetGetWithLiteral);
		ADD_TESTCASE(TestSetGetString);
		ADD_TESTCASE(TestNullKeyFails);
		ADD_TESTCASE(TestNullKeyWithLiteralFails);
		ADD_TESTCASE(TestContainsKeyReturnsTrue);
		ADD_TESTCASE(TestContainsKeyReturnsFalse);
		ADD_TESTCASE(TestContainsValueReturnsTrue);
		ADD_TESTCASE(TestContainsValueReturnsFalse);
		ADD_TESTCASE(TestRemoveKeyRemoves);
		ADD_TESTCASE(TestRemoveKeyThrowsIfNotFound);
		ADD_TESTCASE(TestRemoveValueRemoves);
		ADD_TESTCASE(TestRemoveValueRemovesOnlyFirst);
		ADD_TESTCASE(TestRemoveValueDoesNothingIfNotFound);
		ADD_TESTCASE(TestIndexerPushBack);
		ADD_TESTCASE(TestGetKeyByIndex);
		ADD_TESTCASE(TestGetKeyByIndexFailsWithNegativeIndex);
		ADD_TESTCASE(TestGetKeyByIndexFailsWithTooHighIndex);
		ADD_TESTCASE(TestGetValueByIndex);
		ADD_TESTCASE(TestGetValueByIndexFailsWithNegativeIndex);
		ADD_TESTCASE(TestGetValueByIndexFailsWithTooHighIndex);
		ADD_TESTCASE(TestSortByKeys);
		ADD_TESTCASE(TestSortByValues);
		ADD_TESTCASE(TestSortByValuesMultipleTypes);
		ADD_TESTCASE(TestArrayReferenceWithIncrement);
		ADD_TESTCASE(TestArrayReferenceWithIncrementInSubscript);
		ADD_TESTCASE(TestArrayReferenceWithDoubleIncrement);
	}
};