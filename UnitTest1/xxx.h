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
}wordInfo;//�����ʵĴ���׺ֵ�ͳ��ִ���
typedef struct
{
	string Aword;
	string Bword;
	int appearNum = 0;
}phraselink;//����A�ʺ�B�ʵĴ����ϵ

typedef unordered_map<string, wordInfo> wMap;
typedef unordered_map<string, phraselink> npMap;




//��ȡ���е��ļ���    
void GetAllFiles(string path, vector<string>& files)
{

	long   hFile = 0;
	//�ļ���Ϣ      
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



wMap addWord(string &word, string &word_pre, string &word_r, string &word_pre_r)
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//����map
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
			//������ֺ�׺����
			break;
		}
	}
	postfix = word.substr(wordlen - pfixlen + 1);
	word = word.substr(0, wordlen - pfixlen + 1);//��Сд��word�õ�wordKey
	wordsDic[word].appearNum++;

	if (wordsDic[word].value.empty() || wordsDic[word].value>word_r)
		wordsDic[word].value = word_r;//��¼��ʵֵ

	return wordsDic;
}
npMap addWord2(string &word, string &word_pre, string &word_r, string &word_pre_r)
{
	long charNum = 0;
	long lineNum = 0;
	long wordNum = 0;
	wMap wordsDic;//����map
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
			//������ֺ�׺����
			break;
		}
	}
	postfix = word.substr(wordlen - pfixlen + 1);
	word = word.substr(0, wordlen - pfixlen + 1);//��Сд��word�õ�wordKey
	wordsDic[word].appearNum++;

	if (wordsDic[word].value.empty() || wordsDic[word].value>word_r)
		wordsDic[word].value = word_r;//��¼��ʵֵ

	if (!word.empty() && !word_pre.empty())
	{
		phraseKey = word_pre;
		phraseKey.push_back('-');
		phraseKey += word;
		phraseDic[phraseKey].appearNum++;//�õ�phraseKey
		if (phraseDic[phraseKey].Aword.empty())
		{
			phraseDic[phraseKey].Aword = word_pre_r; phraseDic[phraseKey].Bword = word_r;
		}
		else if (phraseDic[phraseKey].Bword > word_r)
			phraseDic[phraseKey].Bword = word_r;
		else if (phraseDic[phraseKey].Aword > word_pre_r)
			phraseDic[phraseKey].Aword = word_pre_r;
	}
	return phraseDic;
}
