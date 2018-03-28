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
#include <dirent.h>

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

long charNum = 0;
long lineNum = 0;
long wordNum = 0;
wMap wordsDic;//单词map
npMap phraseDic;
wordInfo Mwords[MOSTNUM];
phraselink Mphrases[MOSTNUM];

void listDir(char *path,string & files) //main函数的argv[1] char * 作为 所需要遍历的路径 传参数给listDir
{
	DIR *pDir; //定义一个DIR类的指针
	struct dirent *ent; //定义一个结构体 dirent的指针，dirent结构体见上
	int i = 0;
	char childpath[512]; //定义一个字符数组，用来存放读取的路径
	pDir = opendir(path); // opendir方法打开path目录，并将地址付给pDir指针
	memset(childpath, 0, sizeof(childpath)); //将字符数组childpath的数组元素全部置零
	while ((ent = readdir(pDir)) != NULL)
		//读取pDir打开的目录，并赋值给ent, 同时判断是否目录为空，不为空则执行循环体
	{
		if (ent->d_type & DT_DIR)
			/*读取 打开目录的文件类型 并与 DT_DIR进行位与运算操作，即如果读取的d_type类型为DT_DIR
			(=4 表示读取的为目录)*/
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
				//如果读取的d_name为 . 或者.. 表示读取的是当前目录符和上一目录符,
				//则用contiue跳过，不进行下面的输出
				continue;
			sprintf(childpath, "%s/%s", path, ent->d_name);
			//如果非. ..则将 路径 和 文件名d_name 付给childpath, 并在下一行prinf输出
			//printf("path:%s\n",childpath);原文链接这里是要打印出文件夹的地址
			listDir(childpath);
			//递归读取下层的字目录内容， 因为是递归，所以从外往里逐次输出所有目录（路径+目录名），
			//然后才在else中由内往外逐次输出所有文件名
		}
		else
			//如果读取的d_type类型不是 DT_DIR, 即读取的不是目录，而是文件，
			//则直接输出 d_name, 即输出文件名
		{
			//cout<<ent->d_name<<endl; 输出文件名
			//cout<<childpath<<"/"<<ent->d_name<<endl; 输出带有目录的文件名
			sprintf(childpath, "%s/%s", path, ent->d_name);
			//你可以唯一注意的地方是下一行
			//目前childpath就是你要读入的文件的path了，可以作为你的读入文件的函数的参数
			operate(childpath);//这里就是你的处理文件的接口！，
		}
	}
}

void addWord(string &word, string &word_pre, string &word_r, string &word_pre_r)
{
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
	postfix = word.substr(wordlen - pfixlen);
	word = word.substr(0, wordlen - pfixlen);//从小写的word得到wordKey
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

void operate(char * path)
{
	string word_Breal, word_Areal, word_A, word_B;

	char ch;
	size_t sz;
	FILE*fp;

	{
		/*suffix = files[i].substr(files[i].find_last_of('.') + 1);
		auto itstr = find(mysuffix.begin(), mysuffix.end(), suffix);
		if (itstr == mysuffix.end())
		continue;*/

		//file_test.open(files[i], ios::in);
		//fp =fopen(files[i].c_str(),"r");

		if (fopen_s(&fp, path, "r") != 0)
		{
			cout << "can open file " << path << endl;
			return;
		}
		fseek(fp, 0L, SEEK_END);
		sz = ftell(fp);
		rewind(fp);
		char*buf;
		buf = new char[sz];
		int len = fread(buf, sizeof(char), sz, fp);
		if (len) {
			lineNum++;
		}
		else
			return;

		for (int i = 0; i < len; i++)
		{
			ch = buf[i];
			if (ch >= 32 && ch <= 126)
				charNum++;
			//cout << ch;
			//换行符看成一行
			if (ch >= 'A'&&ch <= 'Z')
			{
				word_Breal.push_back(ch);
				word_B.push_back(ch + 32);
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

		}
		delete[]buf;
		if (fp)
		{
			if (fclose(fp))
			{
				cout << path << " is not closed " << endl;
			}
		}
		/*	file_test.close();
		file_test.clear();*/
	}
}

int main(int argc, char** argv)
{
	string filePath = "C:\\Users\\马睿淳\\Desktop\\测试集与参考结果\\newsample";
	vector<string> files;
	string suffix;
	if (argc < 2)
		cout << "Please input one arguemnt " << endl;
	else
	{
		filePath = argv[1];
		cout << ">>>" << filePath << endl;
	}



	sortWords();
	sortPhrase();

	//输出文件
	ofstream fileout("result我的.txt");
	fileout << "characters:" << charNum << endl;
	fileout << "words:" << wordNum << endl;
	fileout << "lines:" << lineNum << endl;
	//sortMwords(Mwords);
	//sortMphrases(Mphrases);
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

	clock_t b = clock();
	cout << (double)(a - b) / CLOCKS_PER_SEC << "s" << endl;

	return 0;
}
