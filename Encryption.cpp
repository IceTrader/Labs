#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<nlohmann/json.hpp>
#include<ctime>

using namespace std;
using json = nlohmann::json;

/*
	 ________ ___    ___      _______    _______   __      __   ________ __   ________
	|   __   |\  \  /  /     |   __  \  |   __  \ |  |    |  | /  _____/ \ | /  _____/
	|  |__|  | \  \/  /      |  |  \  \ |  |__|  ||  |    |  ||  |  ____  \||  |_____
	|   ___  \  \    /       |  |   |  ||   _   / |  |    |  ||  | |__  \    \_____  \
	|  |___|  |  |  |        |  |__/  / |  | \  \  \  \__/  / |  |___/  /     _____|  |
	|_________|  |__|        |_______/  |__|  \__\  \______/   \_______/     /_______/

*/

class Encryptor
{
public:
	virtual void key_gen() = 0;
	virtual void alph_gen()
	{
		string alphabet;
		cout << "Write string of symbols that your text uses\n";
		cin.get();
		getline(cin, alphabet);
		try
		{
			json j;
			j["TYPE"] = this->type;
			j["ALPH"] = alphabet;
			cout << "Input path and name to new alphabet\n";
			cin >> alphabet;//use free variable
			ofstream out;
			out.open(alphabet + ".alph");
			if (!out.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			out << j;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "\nInvalid alphabet\n";
			return;
		}
	};
	virtual void encrypt() = 0;
	virtual void decrypt() = 0;
protected:
	string type;
	template<typename T>
	struct Encrypted_text//+
	{
		string type;
		T text;
		Encrypted_text(const string& type,const T& text)
		{
			this->text = text;
			this->type = type;
		}
	};
	template<typename T>
	struct Alph_Key//+
	{
		string ALPH;
		T KEY;
		Alph_Key(const string& ALPH,const T& KEY)
		{
			this->ALPH = ALPH;
			this->KEY = KEY;
		}
	};
	template<typename T> T finder(const string& exe, const string& elem)
	{
		json j;
		fstream in;
		string path;
		try
		{
			cout << "Input path to " + elem << endl;
			cin >> path;
			if (path.substr(path.find_last_of('.')) != "." + exe)
			{
				cout << "Wrong file\n";
				abort();
			}
			in.open(path);
			if (!in.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			in >> j;
			if (j["TYPE"].get<string>() != this->type)
			{
				cout << "Incorrect type of " + elem << endl;
				abort();
			}
			in.close();
			return j[elem].get<T>();
		}
		catch (exception e)
		{
			cout << "\nInvalid " + elem << endl;
			abort();
		}
	}
};

class Transposition :public Encryptor //+
{
public:
	Transposition()
	{
		this->type = "Transposition";
	}
	void key_gen()override //+
	{
		srand(time(0));
		int columns = rand() % 6 + 5;
		vector<int> key;
		for (int i = 0; i < columns; i++)
			key.push_back(i);
		int perm_times = rand() % 50 + 51;
		for(int i=0;i<perm_times;i++)
		{
			int iter = rand() % columns;
			int elem = key[iter];
			key.erase(key.begin() + iter);
			key.insert(key.begin() + rand() % columns, elem);
		}
		try
		{
			string path;
			ofstream out;
			json j;
			j["TYPE"] = this->type;
			j["KEY"] = key;
			cout << "Input path and name to new key\n";
			cin >> path;//use free variable
			out.open(path + ".key");
			if (!out.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			out << j;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Something went wrong\n";
			abort();
		}
	}

	void encrypt()override//+
	{
		Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"), finder<vector<int>>("key","KEY"));
		fstream stream;
		string path,text;
		vector<vector<char>>table;
		try
		{
			cout << "Input path and name of text\n";
			cin >> path;
			stream.open(path, ios::in);
			if (!stream.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			vector<char>boof;
			while (!stream.eof())
			{
				getline(stream, path);//use path as variable again
				for(int i=0;i<path.size();i++)
				{
					if (pair.KEY.size() != boof.size())
					{
						boof.push_back(path[i]);
						if (pair.ALPH.find(path[i]) == -1)
						{
							cout << "Wrong alphabet\n";
							abort();
						}
					}
					else
					{
						table.push_back(boof);
						boof.clear();
						boof.push_back(path[i]);
					}
				}
			}
			if(boof.size()!=0)
				table.push_back(boof);
			for (int i = table[table.size() - 1].size(); i < pair.KEY.size(); i++)
				table[table.size() - 1].push_back(32);
			stream.close();
			for (int i = 0; i < pair.KEY.size(); i++)
			{
				for (int j = 0; j < table.size(); j++)
					text.push_back(table[j][pair.KEY[i]]);
			}
			json j;
			j["TYPE"] = this->type;
			j["TEXT"] = text;
			cout << "Input filename to save encrypted text\n";
			cin >> path;
			stream.open(path + ".txt", ios::out);
			stream << j;
			stream.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Wrong input data\n";
			abort();
		}
	}

	void decrypt()override//+
	{
		Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"), finder<vector<int>>("key","KEY"));
		Encrypted_text<string> text(this->type,finder<string>("txt","TEXT"));
		try
		{
			ofstream out;
			string decrypted;
			vector<vector<char>> table(text.text.length() / pair.KEY.size(), vector<char>(pair.KEY.size()));
			int index = 0;
			for (int i = 0; i < pair.KEY.size(); i++)
			{
				for (int j = 0; j < text.text.length() / pair.KEY.size(); j++)
				{
					if (text.text[index] != ' ' && pair.ALPH.find(text.text[index]) == -1)
					{
						cout << "Wrong alphabet\n";
						abort();
					}
					table[j][pair.KEY[i]] = text.text[index];
					index++;
				}
			}
			for (int i = 0; i < table.size(); i++)
				for (int j = 0; j < table[i].size(); j++)
					decrypted.push_back(table[i][j]);
			while (decrypted[decrypted.size()-1]==' ')
			{
				decrypted.pop_back();
			}
			string path;
			cout << "Input filename to save decrypted text\n";
			cin >> path;
			out.open(path + ".txt");
			if (!out.is_open())
			{
				cout << "Couldn`t open or create the file" << endl;
				abort();
			}
			out << decrypted;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Invalid input\n";
			abort();
		}
	}
};

class Gamming :public Encryptor
{
public:
	Gamming()
	{
		this->type = "Gamming";
	}

	virtual void key_gen()override//+
	{
		unsigned int length;
		cout << "Input the length of your message\n";
		cin >> length;
		vector<int> key;
		if(length==0)
		{
			cout << "Length couldn`t be 0\n";
			abort();
		}
		srand(time(0));
		for (int i = 0; i < length; i++)
			key.push_back(rand() % 128);
		try
		{
			string path;
			ofstream out;
			json j;
			j["TYPE"] = this->type;
			j["KEY"] = key;
			cout << "Input path and name to new key\n";
			cin >> path;
			out.open(path + ".key");
			if (!out.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			out << j;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Something went wrong\n";
			abort();
		}
	}

	virtual void encrypt()override//+
	{
		fstream stream;
		string path;
		vector<int>_text;
		Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"),finder<vector<int>>("key","KEY"));
		cout << "Input path and name of text\n";
		cin >> path;
		try
		{
			stream.open(path, ios::in);
			if (!stream.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			while (!stream.eof())
			{
				getline(stream, path);//use path as variable again
				for (int i = 0; i < path.length(); i++)
				{
					if (pair.ALPH.find(path[i]) == -1)
					{
						cout << "Wrong alphabet\n";
						abort();
					}
					_text.push_back(path[i]);
				}
			}
			stream.close();
			if (pair.KEY.size() < _text.size())
			{
				cout << "Invalid key\n";
				abort();
			}
			path = "";
			for (int i = 0; i < _text.size(); i++)
				_text[i] = _text[i] ^ pair.KEY[i];
			Encrypted_text<vector<int>> text(this->type, _text);
			json j;
			j["TYPE"] = text.type;
			j["TEXT"] = text.text;
			cout << "Input filename to save encrypted text\n";
			cin >> path;
			stream.open(path + ".txt", ios::out);
			stream << j;
			stream.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Wrong pair alph-key\n";
			abort();
		}
	}

	virtual void decrypt()override//+
	{
		Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"),finder<vector<int>>("key","KEY"));
		Encrypted_text<vector<int>> text(this->type, finder<vector<int>>("txt", "TEXT"));
		try
		{
			ofstream out;
			string decrypted;
			if(pair.KEY.size()<text.text.size())
			{
				cout << "Invalid key\n";
				abort();
			}
			for (int i = 0; i < text.text.size(); i++)
			{
				decrypted.push_back((char)(text.text[i]^pair.KEY[i]));
				if (pair.ALPH.find(decrypted[i]) == -1)
				{
					cout << "Wrong alphabet\n";
					abort();
				}
			}
			string path;
			cout << "Input filename to save decrypted text\n";
			cin >> path;
			out.open(path + ".txt");
			if (!out.is_open())
			{
				cout << "Couldn`t open or create the file" << endl;
				abort();
			}
			out << decrypted;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Invalid input\n";
			abort();
		}
	}
};

class Replacement :public Encryptor //+
{
public:
	Replacement()
	{
		this->type = "Replacement";
	}
	
	void key_gen()override//+
	{
		string key = finder<string>("alph","ALPH");
		srand(time(0));
		int perm_times = rand() % 67 + 33;
		for (int i = 0; i < perm_times; i++)
		{
			int iter = rand() % (key.length()-1);
			key = key.substr(iter, key.length()) + key.substr(0, iter);
		}
		srand(time(0));
		for (int i = 0; i < perm_times; i++)
		{
			int iter1 = rand() % (key.length() / 2);
			int iter2 = rand() % (key.length() / 2) + key.length() / 2;
			string boof = key.substr(iter1,iter2);
			key.erase(iter1, iter2);
			key.insert(rand() % key.length(), boof);
		}
		try
		{
			json j;
			j["TYPE"] = this->type;
			j["KEY"] = key;
			ofstream out;
			cout << "Input path and name to new key\n";
			cin >> key;//use free variable
			out.open(key + ".key");
			if (!out.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			out << j;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Something went wrong\n";
			abort();
		}
	}

	void encrypt()override//+
	{
		fstream stream;
		string path,text;
		Alph_Key<string> pair(finder<string>("alph", "ALPH"), finder<string>("key", "KEY"));
		cout << "Input path and name of text\n";
		cin >> path;
		try
		{
			stream.open(path,ios::in);
			if(!stream.is_open())
			{
				cout << "Couldn`t open the file\n";
				abort();
			}
			while (!stream.eof())
			{
				getline(stream,path);//use path as variable again
				text = text + path;
			}
			stream.close();
			path = "";
			for (int i = 0; i < text.length(); i++)
			{
				path = path + pair.KEY[pair.ALPH.find(text[i])];
			}
			Encrypted_text<string> text(this->type, path);
			json j;
			j["TYPE"] = text.type;
			j["TEXT"] = text.text;
			cout << "Input filename to save encrypted text\n";
			cin >> path;
			stream.open(path+".txt",ios::out);
			stream << j;
			stream.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Wrong pair alph-key\n";
			abort();
		}
	}

	void decrypt()override//+
	{
		Encrypted_text<string> text(this->type, finder<string>("txt", "TEXT"));
		Alph_Key<string> pair(finder<string>("alph", "ALPH"), finder<string>("key", "KEY"));
		if (pair.ALPH.size() != pair.KEY.size())
		{
			cout << "Wrong pair ahabet-key\n";
			return;
		}
		try
		{
			ofstream out;
			exception e;
			string decrypted;
			for (int i = 0; i < text.text.length(); i++)
			{
				decrypted = decrypted + pair.ALPH[pair.KEY.find(text.text[i])];
			}
			string path;
			cout << "Input filename to save decrypted text\n";
			cin >> path;
			out.open(path + ".txt");
			if (!out.is_open())
			{
				cout << "Couldn`t open or create the file" << endl;
				abort();
			}
			out << decrypted;
			out.close();
			cout << "Done\n";
		}
		catch (exception e)
		{
			cout << "Invalid input" << endl;
			abort();
		}
	}
};

void choicer(Encryptor* e_ptr)//+
{
	int choice;
	while (1)
	{
		cout << "What do you want to do?\n1)Generate the alphabet\n2)Generate the key\n3)Encrypt\n4)Decrypt\n5)Go back\n";
		cin >> choice;
		switch (choice)
		{
			case 1:
			{
				e_ptr->alph_gen();
				break;
			}
			case 2:
			{
				e_ptr->key_gen();
				break;
			}
			case 3:
			{
				e_ptr->encrypt();
				break;
			}
			case 4:
			{
				e_ptr->decrypt();
				break;
			}
			default:
			{
				return;
			}
		}
	}
}

void menu()//+
{
	while (true)
	{
		int choice;
		cout << "Choose the method of encryption\n1)Replacement\n2)Transposition\n3)Gamming\n4)Exit\n";
		cin >> choice;
		switch (choice)
		{
			case 1:
			{
				choicer(new Replacement());
				break;
			}
			case 2:
			{
				choicer(new Transposition());
				break;
			}
			case 3:
			{
				choicer(new Gamming());
				break;
			}
			default:
			{
				return;
			}
		}
	}
}

int main()
{
	menu();
	return 0;
}