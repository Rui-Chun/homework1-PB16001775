#include <io.h>    
#include <cctype>
#include <algorithm>
#include <fstream>    
#include <string>
#include <vector>    
#include <iostream> 
#include <unordered_map>

#define MAX_PATH 100
#define MOSTNUM  10


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

struct HashFunc
{
	std::size_t operator()(const phraselink &key) const
	{
		using std::size_t;
		using std::hash;

		return (hash<string>()(key.Aword) ^ (hash<string>()(key.Bword) << 1));
	}
};
struct EqualKey
{
	bool operator () (const phraselink &lhs, const phraselink &rhs) const
	{
		return lhs.Aword == rhs.Aword
			&& lhs.Bword == rhs.Bword;
	}
};

typedef unordered_map<string, wordInfo> wMap;
typedef unordered_map<phraselink, int, HashFunc, EqualKey>  pMap;
typedef unordered_map<string, phraselink> npMap;

long charNum = 0;
long lineNum = 0;
long wordNum = 0;
wMap wordsDic;//单词map
npMap phraseDic;
wordInfo Mwords[MOSTNUM];
phraselink Mphrases[MOSTNUM];


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

//获取特定格式的文件名    
void GetAllFormatFiles(string path, vector<string>& files, string format)
{
	//文件句柄      
	long   hFile = 0;
	//文件信息      
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );    
					GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
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
	int wordlen = word.length();
	int pfixlen = 0;
	string postfix, phraseKey;
	wMap::iterator tempit;
	for (int i = wordlen - 1;; i--)
	{
		if (word[i]<'0' || word[i]>'9')
		{
			pfixlen = wordlen - 1 - i;
			//获得数字后缀长度
			break;
		}
	}
	postfix = word.substr(wordlen - pfixlen);
	word = word.substr(0, wordlen - pfixlen);//从小写的word得到wordKey
	wordsDic[word].appearNum++;

	if (wordsDic[word].value.empty())
		wordsDic[word].value = word_r;//记录真实值
	else if (wordsDic[word].value>word_r)
		wordsDic[word].value = word_r;

	if (!word.empty() && !word_pre.empty())
	{
		phraseKey = word_pre;
		phraseKey.push_back('-');
		phraseKey = phraseKey.append(word);
		phraseDic[phraseKey].appearNum++;//得到phraseKey
		phraseDic[phraseKey].Aword = word_pre_r;
		phraseDic[phraseKey].Bword = word_r;
	}

}

bool sortWords()
{
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
int main(int argc, char** argv)
{
	string filePath = "C:\\Users\\马睿淳\\Desktop\\测试集与参考结果\\newsample";
	vector<string> files;
	vector<string> mysuffix = { "txt","h","cpp","c","hpp","html","css","js","py" };
	string suffix;


	//读取所有格式为jpg的文件    
	GetAllFiles(filePath, files);


	int size = files.size();
	string word_Breal, word_Areal, word_A, word_B;
	ifstream file_test;

	char ch;



	for (int i = 0; i < size; i++)
	{
		/*suffix = files[i].substr(files[i].find_last_of('.') + 1);
		auto itstr = find(mysuffix.begin(), mysuffix.end(), suffix);
		if (itstr == mysuffix.end())
			continue;*/

		file_test.open(files[i], ios::in);
		if (file_test.get(ch))
			lineNum++;
		else
			continue;
		//判断是否空文件
		do
		{
			if(ch>=32&&ch<127||(ch>=9&&ch<=10))
				charNum++;
			//cout << ch;
			//换行符看成一行
			if (ch >= 'A'&&ch <= 'Z')
			{
				word_B.push_back(ch + 32);
				word_Breal.push_back(ch);
			}
			else if ((ch >= 'a'&&ch <= 'z') || (ch <= '9'&&ch >= '0'))
			{
				word_B.push_back(ch);
				word_Breal.push_back(ch);
			}
			else
			{
				if (ch == '\n')
					lineNum++;	//换行符看成一行
								//transform(word_B.begin(), word_B.end(), word_B.begin(), _tolower_l);
								//transform(word_B.begin(), word_B.end(), word_B.begin(), std::tolower);
								//word_B = tolower(word_B);
				if (isWord(word_B))
				{
					wordNum++;
					addWord(word_B, word_A, word_Breal, word_Areal);
					word_A = word_B;
					word_Areal = word_Breal;
				}
				word_B.clear();
				word_Breal.clear();
			}

		} while (file_test.get(ch));
		//最后读到EOF还有一个单词未处理
		//transform(word_B.begin(), word_B.end(), word_B.begin(), ::tolower);

		if (isWord(word_B))
		{
			addWord(word_B, word_A, word_Breal, word_Areal);
			word_A = word_B;
			wordNum++;
		}
		word_B.clear();

		file_test.close();
		file_test.clear();
	}


	sortWords();
	sortPhrase();

	//输出文件
	ofstream fileout("result我的.txt");
	fileout << "characters:" << charNum << endl;
	fileout << "words:" << wordNum << endl;
	fileout << "lines:" << lineNum << endl;
	sortMwords(Mwords);
	sortMphrases(Mphrases);
	for (int i = 0; i < MOSTNUM; i++)
	{
		if (Mwords[i].appearNum == 0)break;
		fileout << "<" << Mwords[i].value << ">:" << Mwords[i].appearNum << endl;
	}
	for (int i = 0; i < MOSTNUM; i++)
	{
		if (Mphrases[i].appearNum == 0)break;
		fileout << "<" << Mphrases[i].Aword << "-" << Mphrases[i].Bword << ">:" << Mphrases[i].appearNum << endl;
	}
	fileout.close();
	return 0;
}