#pragma once
#include <io.h> 
#include <time.h>
#include <stdio.h>
#include <cctype>
#include <algorithm>
#include <fstream>    
#include <string>
#include <vector>    
#include <iostream> 
#include <unordered_map>

#define MAX_PATH 100
#define MOSTNUM  10

clock_t a = clock();

using namespace std;

typedef struct
{
	string value;
	int appearNum = 0;
}wordInfo;//描述词的带后缀值和出现次数
typedef struct
{
	string Aword;
	string Bword;
	int appearNum = 0;
}phraselink;//描述A词和B词的词组关系

typedef unordered_map<string, wordInfo> wMap;
typedef unordered_map<string, phraselink> npMap;




//获取所有的文件名    
void GetAllFiles(string path, vector<string>& files)
{

	long   hFile = 0;
	//文件信息      
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}

}



void addWord(string &word, string &word_pre, string &word_r, string &word_pre_r)
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//单词map
	npMap phraseDic;
	wordInfo Mwords[MOSTNUM];
	phraselink Mphrases[MOSTNUM];
	int wordlen = word.length() - 1;
	int pfixlen = 0;
	string postfix, phraseKey;
	wMap::iterator tempit;
	for (int i = wordlen;; i--)
	{
		if (word[i]<'0' || word[i]>'9')
		{
			pfixlen = wordlen - i;
			//获得数字后缀长度
			break;
		}
	}
	postfix = word.substr(wordlen - pfixlen + 1);
	word = word.substr(0, wordlen - pfixlen + 1);//从小写的word得到wordKey
	wordsDic[word].appearNum++;

	if (wordsDic[word].value.empty() || wordsDic[word].value>word_r)
		wordsDic[word].value = word_r;//记录真实值

	if (!word.empty() && !word_pre.empty())
	{
		phraseKey = word_pre;
		phraseKey.push_back('-');
		phraseKey += word;
		phraseDic[phraseKey].appearNum++;//得到phraseKey
		if (phraseDic[phraseKey].Aword.empty())
		{
			phraseDic[phraseKey].Aword = word_pre_r; phraseDic[phraseKey].Bword = word_r;
		}
		else if (phraseDic[phraseKey].Bword > word_r)
			phraseDic[phraseKey].Bword = word_r;
		else if (phraseDic[phraseKey].Aword > word_pre_r)
			phraseDic[phraseKey].Aword = word_pre_r;
	}

}

bool sortWords()
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//单词map
	npMap phraseDic;
	wordInfo Mwords[MOSTNUM];
	phraselink Mphrases[MOSTNUM];
	wMap::iterator temp = wordsDic.begin();
	unsigned int sortNum = MOSTNUM;
	if (wordsDic.size() < sortNum)
		sortNum = wordsDic.size();
	for (unsigned int i = 0; i < sortNum; i++)
	{
		temp = wordsDic.begin();
		for (wMap::iterator it = wordsDic.begin(); it != wordsDic.end(); it++)
		{
			if ((it->second.appearNum > temp->second.appearNum) || (it->second.appearNum == temp->second.appearNum && it->first < temp->first))//这里应该比较key还是真值的大小
			{
				temp = it;
			}
		}
		Mwords[i] = temp->second;
		wordsDic.erase(temp);
	}
	return true;
}
bool sortPhrase()
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//单词map
	npMap phraseDic;
	wordInfo Mwords[MOSTNUM];
	phraselink Mphrases[MOSTNUM];
	npMap::iterator temp = phraseDic.begin();
	unsigned int sortNum = MOSTNUM;
	if (phraseDic.size() < sortNum)
		sortNum = phraseDic.size();

	for (unsigned int i = 0; i < sortNum; i++)
	{
		temp = phraseDic.begin();
		for (auto it = phraseDic.begin(); it != phraseDic.end(); it++)
		{
			if (it->second.appearNum > temp->second.appearNum || (it->second.appearNum == temp->second.appearNum&&it->first < temp->first))
				temp = it;
		}
		Mphrases[i] = temp->second;
		phraseDic.erase(temp);
	}
	return true;
}
bool isWord(string word)
{
	for (int i = 0; i < 4; i++)
	{
		if (word[i]<'a' || word[i]>'z')
			return false;
	}
	return true;
}

wordInfo* sortMwords(wordInfo* Mwords)
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//单词map
	npMap phraseDic;
	phraselink Mphrases[MOSTNUM];
	wordInfo temp = Mwords[0];
	for (int i = 0; i < MOSTNUM; i++)
	{
		temp = Mwords[i];
		if (Mwords[i].appearNum == 0)break;
		for (int j = i + 1; j < MOSTNUM; j++)
		{
			if (Mwords[j].appearNum == 0)break;
			if (Mwords[j].value < temp.value)
			{
				temp = Mwords[j];
				Mwords[j] = Mwords[i];
				Mwords[i] = temp;
			}
		}
	}
	return Mwords;
}
phraselink* sortMphrases(phraselink* Mphrases)
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//单词map
	npMap phraseDic;
	wordInfo Mwords[MOSTNUM];
	phraselink temp = Mphrases[0];
	for (int i = 0; i < MOSTNUM; i++)
	{
		temp = Mphrases[i];
		if (Mphrases[i].appearNum == 0)break;
		for (int j = i + 1; j < MOSTNUM; j++)
		{
			if (Mphrases[j].appearNum == 0)break;
			if (Mphrases[j].Aword < temp.Aword || (Mphrases[j].Aword == temp.Aword) && Mphrases[j].Bword < temp.Bword)
			{
				temp = Mphrases[j];
				Mphrases[j] = Mphrases[i];
				Mphrases[i] = temp;
			}
		}
	}
	return Mphrases;
}
string tolower(string & str)
{
	int len = str.length();
	for (int i = 0; i < len; i++)
		if (str[i] >= 'A'&&str[i] <= 'Z')
			str[i] = str[i] + 32;
	return str;
}//相比较库函数改进有限