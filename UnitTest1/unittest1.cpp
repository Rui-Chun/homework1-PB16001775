#include "stdafx.h"
#include "CppUnitTest.h"
#include "xxx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(ADDWORD)
	{
	public:

		TEST_METHOD(Postfix)
		{
			// TODO: 在此输入测试代码
			string wrod233 = "windows123", word_r = "windows123", word_pre = "", word_pre_r = "";
			wMap dic = addWord(wrod233, word_pre, word_r, word_pre_r);
			Assert::AreEqual("windows123", dic["windows"].value.c_str());
		}
		TEST_METHOD(Order)
		{
			string wrod233 = "windows123", word_r = "Windows", word_pre = "", word_pre_r = "";
			wMap dic = addWord(wrod233, word_pre, word_r, word_pre_r);
			Assert::AreEqual("Windows", dic["windows"].value.c_str());
		}
		TEST_METHOD(Value)
		{
			string wrod233 = "windows123", word_r = "Windows", word_pre = "", word_pre_r = "";
			addWord(wrod233, word_r, word_pre, word_pre_r);
			wMap dic = addWord(wrod233, word_pre, word_r, word_pre_r);
			Assert::AreEqual("Windows", dic["windows"].value.c_str());
		}
		TEST_METHOD(phrase)
		{
			string wrod233 = "windows123", word_r = "Windows", word_pre = "what", word_pre_r = "what233";
			npMap dic = addWord2(wrod233, word_pre, word_r, word_pre_r);
			Assert::AreEqual("what233", dic["what-windows"].Aword.c_str());
			Assert::AreEqual("Windows", dic["what-windows"].Bword.c_str());
		}

	};
}