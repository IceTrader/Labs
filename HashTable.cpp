#include <iostream>
#include<string>
#include"LinkedList.h"
#include<vector>
#include<ctime>
#include<bitset>
#include <iomanip>
#include<math.h>
#include<sstream>

using namespace std;

class Pair
{
public:
	string get_hash()
	{
		return this->hash;
	}
	vector<string>get_data()
	{
		return this->vec;
	}
	int get_hash_collisiums()
	{
		return vec.size() - 1;
	}
	bool is_empty()
	{
		if (this->vec.size() == 0)
			return true;
		return false;
	}
	Pair(){}
	Pair makepair(string data,const int &x)
	{		
		Pair pair;
		pair.set_hash(data,x);
		return pair;
	}
	void add_collisium(string data)
	{
		vec.push_back(data);
	}
private:
	string hash;
	vector<string> vec;
	static string HASH(string& data,const int&x);
	void set_hash(string& data, const int& x)
	{
		this->hash = HASH(data,x);
		this->vec.push_back(data);
	}
};
string Pair::HASH(string& data,const int &x)
{
	string hash;
	for(int i=0;i<128;i++)
	{
		if(data.size()>i)
		{
			bitset<8> bithash(data[i]);
			hash += bithash.to_string();
		}
		else
		{
			bitset<8> bithash(x);
			if (i % 3 == 1)
				~bithash;
			if (i % 3 == 2)
				bithash^=x;
			hash += bithash.to_string();
		}
	}
	bitset<1024>bithash(hash);
	bithash >>= (x%5);
	bithash |= x;
	bithash ^= x;
	string bin(bithash.to_string());
	hash = "";
	for(int i=0;i<bin.size();i=i+4)
	{
		string buf = to_string(bin[i]-48) + to_string(bin[i+1]-48) + to_string(bin[i+2]-48) + to_string(bin[i+3]-48);
		if (buf == "0000")
			hash += "0";
		else if (buf == "0001")
			hash += "1";
		else if (buf == "0010")
			hash += "2";
		else if (buf == "0011")
			hash += "3";
		else if (buf == "0100")
			hash += "4";
		else if (buf == "0101")
			hash += "5";
		else if (buf == "0110")
			hash += "6";
		else if (buf == "0111")
			hash += "7";
		else if (buf == "1000")
			hash += "8";
		else if (buf == "1001")
			hash += "9";
		else if (buf == "1010")
			hash += "A";
		else if (buf == "1011")
			hash += "B";
		else if (buf == "1100")
			hash += "C";
		else if (buf == "1101")
			hash += "D";
		else if (buf == "1110")
			hash += "E";
		else if(buf == "1111")
			hash += "F";
	}
	return hash;
}
class HashTable : public LinkedList<Pair>
{
public:
	HashTable() 
	{
		srand(time(0));
		this->x=rand();
	}
	~HashTable() {}
	void printtable(HashTable &current)  
	{
		for (int i = 0; i < this->get_size(); i++)
		{
			Pair pair = current[i];
			cout << "[" << i << "] has( "<< pair.get_hash_collisiums()<<" ) collisiums and hash : \n"<<pair.get_hash()<<endl;
		}	
	}
	void add(string data)
	{
		Pair pair=pair.makepair(data,this->x);
		Pair* ptr = exist(pair.get_hash());
		if (ptr != nullptr)
		{
			if (this->data_exist(pair,ptr))
				return;
			ptr->add_collisium(data);
		}
		else
			LinkedList<Pair>::add(pair);
	}
private:
	bool data_exist(Pair& E1, Pair* E2)
	{
		for (int i = 0; i < E2->get_data().size(); i++)
		{
			for (int j = 0; j < E1.get_data().size(); j++)
				if (E1.get_data()[j] == E2->get_data()[i])
					return true;
		}
		return false;
	}
	Pair* exist(string hash)
	{
		if (this->HEAD == nullptr)
			return nullptr;
		Element<Pair>* current=this->HEAD;
		for (int i = 0; i < this->Size; i++)
		{
			if ((current->data).get_hash() == hash)
			{
				Pair &pair = current->data;
				return &pair;
			}
			else
				current = current->next_address;
		}
		return nullptr;
	}
	int x;
};

int main()
{
	HashTable MyTable;
	/*for (int i = 0; i < 95; i++)
	{
		MyTable.add(to_string(i));
		cout << i << endl;
	}*/
	MyTable.add("abcd");
	cout << "it works"<<endl;
	MyTable.printtable(MyTable);
	return 0;
}