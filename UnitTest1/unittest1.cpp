#include "stdafx.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(ADDWORD)
	{
	public:
		
		TEST_METHOD(Postfix)
		{
			// TODO: 在此输入测试代码
			string wrod233 = "windows123",word_r="windows123",word_pre="",word_pre_r="";
			wMap dic=addWord(wrod233, word_r, word_pre, word_pre_r);
			Assert::AreEqual("windows123",dic["windows"].value.c_str());
		}
		TEST_METHOD(Order)
		{
			string wrod233 = "windows123", word_r = "Windows", word_pre = "", word_pre_r = "";
			wMap dic = addWord(wrod233, word_r, word_pre, word_pre_r);
			Assert::AreEqual("Windows", dic["windows"].value.c_str());
		}
		TEST_METHOD(Value)
		{
			string wrod233 = "windows123", word_r = "Windows", word_pre = "", word_pre_r = "";
			addWord(wrod233, word_r, word_pre, word_pre_r);
			wMap dic=addWord(wrod233, word_r, word_pre, word_pre_r);
			Assert::AreEqual("Windows", dic["windows"].value.c_str());
		}
		TEST_METHOD(TestMethod4)
		{

		}
	};
	TEST_CLASS(ELSEFUNC)
	{

	};
}