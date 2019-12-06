#include <iostream>
#include<string>
#include<vector>
#include <cmath>
using namespace std;

template<typename T>
class WorldTree
{
	public:
		WorldTree();//+
		~WorldTree();//+
		string show_type(); //+
		int get_capacity() 
		{
			return Capacity;
		}			
		//+
		void cut_down();//+
		void grow(T knowledge); //+
		bool exist(T knowledge);//+
		bool exist(string key);//+
		string & operator[](T knowledge);//+  keys
		void dispel(string key);//+
		void dispel(T knowledge);//+
		void regrow(string key);//+
		void regrow(T knowledge);//+
		

	private:
		void dispel(string rootkey, string key,vector<T>& v);//+
		template<typename T>
		class Wisp
		{
		public:
			T knowledge;
			string key;
			Wisp* trunk;
			Wisp* right_branch;
			Wisp* left_branch;
			Wisp(string key, T knowledge = T(), Wisp* trunk = nullptr)
			{
				this->knowledge = knowledge;
				this->key = key;
				this->trunk = trunk;
				this->right_branch = nullptr;
				this->left_branch = nullptr;
			}
		};
		int Capacity;
		Wisp<T>* ROOT;
		void get_vector(vector<Wisp<T>*>& v, Wisp<T>* wisp,int level)
		{
			
			if (wisp != nullptr)
			{
					get_vector(v, wisp->left_branch, level - 1);
					if (level == 1)
						v.push_back(wisp);
					get_vector(v,wisp->right_branch,level -1);
			}
		}
	public:
		Wisp<T>* get_root()
		{
			return this->ROOT;
		}
		void print(unsigned int level=1)
		{
			if (this->Capacity == 0)
			{
				cout << "Couldn`t write an empty tree" << endl;
				abort();
			}
			else
			{
				vector<Wisp<T>*> v;
				Wisp<T>* nullwisp = new Wisp<T>("NULL", 0);
				int real_level = 0;
				for(int i=1;i<=level;i++)
				{
					get_vector(v, ROOT, i);
					if(v[v.size()-1]!=nullwisp)
					{
						v.push_back(nullwisp);
						real_level++;
					}
				}
				for (int i = 0; i < v.size(); i++)
				{
					if (v[i]->key != "NULL")
					{
						for (int a = pow(2, real_level - 1);a>0;a--)
							cout << "\t";
							cout << v[i]->knowledge;
						for (int a = pow(2, real_level - 1); a > 0; a--)
							cout << "\t";
					}
					else
					{
						real_level--;
						cout << endl;
					}
				}
			}
		}
};

template<typename T>
WorldTree<T>::WorldTree()
{	
	this->Capacity = 0;
	this->ROOT = nullptr;
}

template<typename T>
WorldTree<T>::~WorldTree()
{
	if(exist("ROOT"))
		cut_down();
}

template<typename T>
string WorldTree<T>::show_type()
{
	return typeid(T).name();
}

template<typename T>
void WorldTree<T>::cut_down()
{
	dispel(ROOT->key);
}

template<typename T>
void WorldTree<T>::grow(T knowledge)
{
	if (ROOT == nullptr)
		ROOT = new Wisp<T>("ROOT",knowledge);
	else
	{
		if(exist(knowledge))
		{
			cout << "Tree already have this!" << endl;
			return;
		}
		Wisp<T>* current = ROOT;
		string key_counter;
		for (int i=1;;i++)
		{
			if (knowledge > current->knowledge  && current->right_branch==nullptr)
			{
				key_counter += "1";
				current->right_branch = new Wisp<T>(key_counter,knowledge, current);
				break;
			}
			else if(knowledge < current->knowledge && current->left_branch == nullptr)
			{
				key_counter += "0";
				current->left_branch = new Wisp<T>(key_counter,knowledge,current);
				break;
			}
			else if (knowledge > current->knowledge&& current->right_branch != nullptr)
			{
				current = current->right_branch;
				key_counter += "1";
			}
			else 
			{
				key_counter += "0";
				current = current->left_branch;
			}
		}
	}
	Capacity++;
}

template<typename T>
bool WorldTree<T>::exist(T knowledge)
{
	if (ROOT == nullptr)
		return false;
	Wisp<T>* current = ROOT;
	while (current!=nullptr)
	{
		if(current->knowledge==knowledge)
			return true;
		if (current->left_branch == current->right_branch)
			return false;
		if (knowledge > current->knowledge)
			current = current->right_branch;
		else
			current = current->left_branch;
	}
	return false;
}

template<typename T>
bool WorldTree<T>::exist(string key)
{
	if (ROOT == nullptr)
		return false;
	Wisp<T>* current = ROOT;
	for (int i = 0;current != nullptr;i++)
	{
		if (current->key == key)
			return true;
		if (current->left_branch == current->right_branch)
			break;
		if (key[i] == '1')
			current = current->right_branch;
		else
			current = current->left_branch;
	}
	return false;
}

template<typename T>
string & WorldTree<T>::operator[](T knowledge)
{
	Wisp<T>* current = this->ROOT;
	while(this->exist(knowledge))
	{
			if (current->knowledge == knowledge)
				return current->key;
			if (knowledge > current->knowledge)
				current = current->right_branch;
			else
				current = current->left_branch;
	}
	abort();
}

template<typename T>
void WorldTree<T>::dispel(string key)
{
	if (!exist(key))
		cout << "Nothing to dispel!" << endl;
	else
	{
		Wisp<T>* current = ROOT;
		for (int i = 0; current->key != key; i++)
		{
			if (key[i] == '1')
				current = current->right_branch;
			else
				current = current->left_branch;
		}
		if (current->left_branch == nullptr && current->right_branch == nullptr)
		{
			if (current == ROOT)
			{
				ROOT = nullptr;
				delete current;
			}
			else
			{
				if (current->trunk->left_branch == current)
				{
					current->trunk->left_branch = nullptr;
					delete current;
				}
				else
				{
					current->trunk->right_branch = nullptr;
					delete current;
				}
			}
			Capacity--;
		}
		else if (current->left_branch != nullptr && current->right_branch == nullptr)
		{
			dispel(current->left_branch->key);
			dispel(current->key);
		}
		else if (current->left_branch == nullptr && current->right_branch != nullptr)
		{
			dispel(current->right_branch->key);
			dispel(current->key);
		}
		else
		{
			dispel(current->left_branch->key);
			dispel(current->right_branch->key);
			dispel(current->key);
		}
	}
}

template<typename T>
void WorldTree<T>::dispel(string rootkey,string key,vector<T>&v)
{
	if (!exist(key))
		cout << "Nothing to dispel!" << endl;
	else
	{
		Wisp<T>* current = ROOT;
		for(int i=0;current->key != key;i++)
		{
			if (key[i] == '1')
				current = current->right_branch;
			else
				current = current->left_branch;
		}
		if(current->left_branch==nullptr&& current->right_branch==nullptr)
		{
			if (current == ROOT) 
			{
				ROOT = nullptr;
				delete current;
			}
			else if (key == rootkey) 
			{
				if (current->trunk->left_branch == current)
				{
					current->trunk->left_branch = nullptr;
					delete current;
				}
				else
				{
					current->trunk->right_branch = nullptr;
					delete current;
				}
			}
			else
			{
				if (current->trunk->left_branch == current)
				{
					current->trunk->left_branch = nullptr;
					v.push_back(current->knowledge);
					delete current;
				}
				else
				{
					current->trunk->right_branch = nullptr;
					v.push_back(current->knowledge);
					delete current;
				}
			}
			Capacity--;
		}
		else if (current->left_branch != nullptr && current->right_branch == nullptr)
		{
			dispel(rootkey,current->left_branch->key,v);
			dispel(rootkey,current->key,v);
		}
		else if (current->left_branch == nullptr && current->right_branch != nullptr) 
		{
			dispel(rootkey,current->right_branch->key,v);
			dispel(rootkey,current->key,v);
		}
		else
		{
			dispel(rootkey,current->left_branch->key,v);
			dispel(rootkey,current->right_branch->key,v);
			dispel(rootkey,current->key,v);
		}
	}
}

template<typename T>
void WorldTree<T>::dispel(T knowledge)
{
	if (!exist(knowledge))
		cout << "Nothing to dispel!" << endl;
	else
	{
		Wisp<T>* current = ROOT;
		while(current->knowledge != knowledge)
		{
			if (knowledge>current->knowledge)
				current = current->right_branch;
			else
				current = current->left_branch;
		}
		dispel(current->key);
	}
}

template<typename T>
void WorldTree<T>::regrow(string key)
{
	if (!exist(key))
		cout << "Nothing to regrow!" << endl;
	else
	{
		Wisp<T>* del = ROOT;
		for (int i = 0; del->key != key; i++)
		{
			if (key[i] == '1')
				del = del->right_branch;
			else
				del = del->left_branch;
		}
		vector<T>data;
		dispel(del->key,del->key, data);
		while (data.size() > 1)
		{
			grow(data[data.size()/2]);
			data.erase(data.begin()+data.size()/2);
		}
		if(data.size()==1)
			grow(data[0]);
	}
}

template<typename T>
void WorldTree<T>::regrow(T knowledge)
{
	if (!exist(knowledge))
		cout << "Nothing to regrow!" << endl;
	else
	{
		Wisp<T>* current = ROOT;
		while(current->knowledge != knowledge)
		{
			if (knowledge > current->knowledge)
				current = current->right_branch;
			else
				current = current->left_branch;
		}
		regrow(current->key);
	}
}


int main()
{
	WorldTree<int> tree;
	tree.grow(10);
	tree.grow(5);
	tree.grow(20);
	tree.grow(1);
	tree.grow(6);
	tree.grow(15);
	tree.grow(21);

	tree.print(4111);
	
	return 0;
}
