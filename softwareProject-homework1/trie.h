#include <iostream>
#include <string>
#define CHARNUM 40
#define MOSTNUM 10
using namespace std;

class TrieNode
{
public:
	int count;
	bool isword;
	string postfix;
	TrieNode * next[CHARNUM];
	TrieNode() :count(0), isword(false)
	{
		for (int i = 0; i < CHARNUM; i++)
			next[i] = NULL;
	}
	~TrieNode()
	{
		for (int i = 0; i < CHARNUM; i++)
		{
			delete next[i];
			next[i] = NULL;
		}
	}
};

typedef struct
{
	string mword;
	long appearNum=0;
}mostWords;

typedef struct
{
	string aword;
	string bword;
	long appearNum = 0;
}mostPhrases;

class Trie
{
private:
	TrieNode * root;
	int wordNum;
	mostWords Mwords[MOSTNUM];
	mostPhrases Mphrases[MOSTNUM];
private:
	bool changeMwords(string word,int count)
	{
		if (word.length() == 0)return false;
		for (int i = 0; i < MOSTNUM; i++)
		{
			if (count > Mwords[i].appearNum)
			{
				Mwords[i].mword = word;
				Mwords[i].appearNum = count;
				return true;
			}
		}
	}
public:
	Trie()
	{
		root = new TrieNode;
		wordNum = 0;
	}
	~Trie()
	{
		if (root)
		{
			delete root;
			root = NULL;
		}
	}

	long wordNum() { return wordNum; }
	bool addWord(string word)
	{
		//默认传入的word符合单词要求
		TrieNode* p = root;
		int index = 0;
		int wordlen = word.length();
		int pfixlen = 0;
		for (int i = wordlen - 1;; i--)
		{
			if (word[i]<'0' || word[i]>'9')
			{
				pfixlen = wordlen - 1 - i;
				//获得数字后缀长度
				break;
			}
		}

		for (int i = 0; i < wordlen - pfixlen; i++)
		{
			if (word[i] > 'A'&&word[i] <= 'Z')
				index = word[i] - 'A' + 10;
			else if (word[i] > 'a'&&word[i] <= 'z')
				index = word[i] - 'a' + 10;
			else
				index = word[i] - '0';

			if (p->next[index] == NULL)
				p->next[index] = new TrieNode;
			p = p->next[index];
			if (i == wordlen - pfixlen - 1)
			{
				p->count++;
				p->isword = true;
				p->postfix = word.substr(i + 1);
				changeMwords(word, p->count);
			}
		}
		return true;
	}
};