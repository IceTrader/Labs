#include <iostream>
#include<string>
#include<vector>

using namespace std;

string sdvig(string & r,vector<int>&mas)
{
	string res;
	for(int i=0;i<r.size();i++)
		res += char(int(r[i]) - mas[i % (mas.size())]);
	return res;
}
vector<int> key(unsigned long key)
{
	int razr = 0;
	for (int i = key; i > 0; i /= 10)razr++;
	vector<int> v(razr);
	for (int i = razr - 1; i >= 0; i--)
	{
		v[i] = key % 10;
		key /= 10;
	}
	return v;
}
void Find(string & ref,const vector<string> &vec, unsigned long &KEY)
{
	for(int i=0;i<vec.size();i++)
	{
		if (ref.length() < vec[i].length())
			continue;
		for (int j = 0; j <= ref.length() - vec[i].length(); j++)
		{
			int c = 0;
			for(int k=j;k<j+vec[i].length();k++)
			{
				if (vec[i][k - j] != ref[k])
					break;
				c++;
				if (c == vec[i].length())
					cout << "Find " << vec[i] << " in "<< ref <<" with KEY "<<KEY<<endl;
			}
		}
	}
}
void crack(string & r, const vector<string>& vect)
{
	cout << "Your string " << r << endl;
	for (unsigned long KEY = 1; KEY < 4294967295; KEY++)
	{
		vector<int> v = key(KEY);
		string buf = sdvig(r, v);
		Find(buf, vect,KEY);
	}
}
int main()
{
	vector<string>dictionary;
	dictionary.push_back("car");
	dictionary.push_back("hello");
	dictionary.push_back("nigga");
	dictionary.push_back("pen");
	dictionary.push_back("ball");
	string x;
	getline(cin,x);
	crack(x,dictionary);
}