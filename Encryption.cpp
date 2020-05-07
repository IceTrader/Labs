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
		while (true)
		{
			cout << "Write string of symbols that your text uses\n";
			cin.get();
			getline(cin, alphabet);
			if (!cin.good())
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid symbols found.Try to input correct symbols!\n";
			}
			else
				break;
		}
		try
		{
			json j;
			j["TYPE"] = this->type;
			j["ALPH"] = alphabet;
			ofstream out;
			while(1)
			{
				cout << "Input path and name to new alphabet\n";
				cin >> alphabet;//use free variable
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				out.open(alphabet + ".alph");
				if (out.is_open())
					break;
				cout << "Couldn`t open the file.Try to input another path and name\n";
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
		while (true)
		{
			try
			{
				cout << "Input path to " + elem << endl;
				cin >> path;
				if(!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				if (path.substr(path.find_last_of('.')) != "." + exe)
				{
					cout << "Wrong file\n";
					continue;
				}
				in.open(path);
				if (!in.is_open())
				{
					cout << "Couldn`t open the file\n";
					continue;
				}
				in >> j;
				if (j["TYPE"].get<string>() != this->type)
				{
					cout << "Incorrect type of " + elem << endl;
					continue;
				}
				in.close();
				return j[elem].get<T>();
			}
			catch (exception e)
			{
				cout << "\nInvalid " + elem << endl;
				continue;
			}
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
		int columns;
		char c;
		while(true)
		{
			cout << "Do you want to set number of columns by yourself?(Y/N)\n";
			cin >> c;
			if (!cin.good())
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid symbols found!\n";
				continue;
			}
			if(c=='Y')
			{
				cout << "Input number of columns\n";
				cin >> columns;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found!\n";
					continue;
				}
				if(columns<1)
				{
					cout << "Number of columns couldn`t be less than 1\n";
					continue;
				}
				break;
			}
			else
			{
				srand(time(0));
				columns = rand() % 6 + 5;
				break;
			}
		}
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
		while (true)
		{
			try
			{
				string path;
				ofstream out;
				json j;
				j["TYPE"] = this->type;
				j["KEY"] = key;
				cout << "Input path and name to new key\n";
				cin >> path;//use free variable
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				out.open(path + ".key");
				if (!out.is_open())
				{
					cout << "Couldn`t open the file\n";
					continue;
				}
				out << j;
				out.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Something went wrong\n";
				continue;
			}
		}
	}

	void encrypt()override//+
	{
		bool good;
		while (true)
		{
			good = true;
			Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"), finder<vector<int>>("key","KEY"));
			fstream stream;
			string path,text;
			vector<vector<char>>table;
			try
			{
				cout << "Input path and name of text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				stream.open(path, ios::in);
				if (!stream.is_open())
				{
					cout << "Couldn`t open the file\n";
					continue;
				}
				vector<char>boof;
				while (!stream.eof())
				{
					getline(stream, path);//use path as variable again
					for (int i = 0; i < path.size(); i++)
					{
						if (pair.KEY.size() != boof.size())
						{
							boof.push_back(path[i]);
							if (pair.ALPH.find(path[i]) == -1)
							{
								cout << "Wrong alphabet\n";
								good= false;
							}
							if (!good)break;
						}
						else
						{
							table.push_back(boof);
							boof.clear();
							boof.push_back(path[i]);
						}
						if (!good)break;
					}
					if (!good)break;
				}
				if (!good)continue;
				if (boof.size() != 0)
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
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				stream.open(path + ".txt", ios::out);
				while(!stream.is_open())
				{
					cout << "Couldn`t open the file\n";
					cout << "Input filename to save encrypted text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					stream.open(path + ".txt", ios::out);
				}
				stream << j;
				stream.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Wrong input data\n";
				continue;
			}
		}
	}

	void decrypt()override//+
	{
		bool good;
		while (true)
		{
			good = true;
			Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"), finder<vector<int>>("key", "KEY"));
			Encrypted_text<string> text(this->type, finder<string>("txt", "TEXT"));
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
							good = false;
						}
						if (!good)break;
						table[j][pair.KEY[i]] = text.text[index];
						index++;
					}
					if (!good)break;
				}
				if (!good)continue;
				for (int i = 0; i < table.size(); i++)
					for (int j = 0; j < table[i].size(); j++)
						decrypted.push_back(table[i][j]);
				while (decrypted[decrypted.size() - 1] == ' ')
				{
					decrypted.pop_back();
				}
				string path;
				cout << "Input filename to save decrypted text\n";
				cin >> path;
				out.open(path + ".txt");
				while(!out.is_open())
				{
					cout << "Couldn`t open or create the file" << endl;
					cout << "Input filename to save decrypted text\n";
					cin >> path;
					out.open(path + ".txt");
				}
				out << decrypted;
				out.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Invalid input\n";
				continue;
			}
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
		while (true)
		{
			int length;
			cout << "Input the length of your key\n";
			cin >> length;
			if (!cin.good())
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid symbols found.Try to input correct symbols!\n";
				continue;
			}
			vector<int> key;
			if (!length > 0)
			{
				cout << "Length couldn`t be less than 1\n";
				continue;
			}
			srand(time(0));
			for (int i = 0; i < length; i++)
				key.push_back(rand() % 128);
			while (true)
			{
				try
				{
					string path;
					ofstream out;
					json j;
					j["TYPE"] = this->type;
					j["KEY"] = key;
					cout << "Input path and name to new key\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					out.open(path + ".key");
					while(!out.is_open())
					{
						cout << "Couldn`t open the file\n";
						cout << "Input path and name to new key\n";
						cin >> path;
						if (!cin.good())
						{
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Invalid symbols found.Try to input correct symbols!\n";
							continue;
						}
						out.open(path + ".key");
					}
					out << j;
					out.close();
					cout << "Done\n";
					return;
				}
				catch (exception e)
				{
					cout << "Something went wrong\n";
					continue;
				}
			}
		}
	}

	virtual void encrypt()override//+
	{
		fstream stream;
		string path;
		vector<int>_text;
		bool good;
		while (true)
		{
			good = true;
			Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"), finder<vector<int>>("key", "KEY"));
			try
			{
				cout << "Input path and name of text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				stream.open(path, ios::in);
				while(!stream.is_open())
				{
					cout << "Couldn`t open the file\n";
					cout << "Input path and name of text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					stream.open(path, ios::in);
				}
				while (!stream.eof())
				{
					getline(stream, path);//use path as variable again
					for (int i = 0; i < path.length(); i++)
					{
						if (pair.ALPH.find(path[i]) == -1)
						{
							cout << "Wrong alphabet\n";
							good = false;
						}
						if (!good)break;
						_text.push_back(path[i]);
					}
					if (!good)break;
				}
				if (!good)continue;
				stream.close();
				path = "";
				for (int i = 0; i < _text.size(); i++)
					_text[i] = _text[i] ^ pair.KEY[i%pair.KEY.size()];
				Encrypted_text<vector<int>> text(this->type, _text);
				json j;
				j["TYPE"] = text.type;
				j["TEXT"] = text.text;
				cout << "Input filename to save encrypted text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				stream.open(path + ".txt", ios::out);
				while(!stream.is_open())
				{
					cout << "Couldn`t open the file\n";
					cout << "Input filename to save encrypted text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
				}
				stream << j;
				stream.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Wrong pair alph-key\n";
				continue;
			}
		}
	}

	virtual void decrypt()override//+
	{
		bool good;
		while (true)
		{
			good = true;
			Alph_Key<vector<int>> pair(finder<string>("alph", "ALPH"), finder<vector<int>>("key", "KEY"));
			Encrypted_text<vector<int>> text(this->type, finder<vector<int>>("txt", "TEXT"));
			try
			{
				ofstream out;
				string decrypted;
				for (int i = 0; i < text.text.size(); i++)
				{
					decrypted.push_back((char)(text.text[i] ^ pair.KEY[i%pair.KEY.size()]));
					if (pair.ALPH.find(decrypted[i]) == -1)
					{
						cout << "Wrong alphabet\n";
						good = false;
						break;
					}
				}
				if (!good)continue;
				string path;
				cout << "Input filename to save decrypted text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				out.open(path + ".txt");
				while(!out.is_open())
				{
					cout << "Couldn`t open or create the file" << endl;
					cout << "Input filename to save decrypted text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					out.open(path + ".txt");
				}
				out << decrypted;
				out.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Invalid input\n";
				continue;
			}
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
		while (true)
		{
			try
			{
				json j;
				j["TYPE"] = this->type;
				j["KEY"] = key;
				ofstream out;
				cout << "Input path and name to new key\n";
				cin >> key;//use free variable
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				out.open(key + ".key");
				while(!out.is_open())
				{
					cout << "Couldn`t open the file\n";
					cout << "Input path and name to new key\n";
					cin >> key;//use free variable
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					out.open(key + ".key");
				}
				out << j;
				out.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Something went wrong\n";
				continue;
			}
		}
	}

	void encrypt()override//+
	{
		fstream stream;
		string path,text;
		bool good;
		while (true)
		{
			good = true;
			Alph_Key<string> pair(finder<string>("alph", "ALPH"), finder<string>("key", "KEY"));
			try
			{ 
				cout << "Input path and name of text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				stream.open(path, ios::in);
				while(!stream.is_open())
				{
					cout << "Couldn`t open the file\n";
					cout << "Input path and name of text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					stream.open(path, ios::in);
				}
				while (!stream.eof())
				{
					getline(stream, path);//use path as variable again
					text = text + path;
				}
				stream.close();
				path = "";
				for (int i = 0; i < text.length(); i++)
				{
					if(pair.ALPH.find(text[i])==-1)
					{
						good = false;
						cout << "Invalid symbols found!\n";
						break;
					}
					path = path + pair.KEY[pair.ALPH.find(text[i])];
				}
				if (!good)continue;
				Encrypted_text<string> text(this->type, path);
				json j;
				j["TYPE"] = text.type;
				j["TEXT"] = text.text;
				cout << "Input filename to save encrypted text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				stream.open(path + ".txt", ios::out);
				while (!stream.is_open())
				{
					cout << "Couldn`t open the file\n";
					cout << "Input filename to save encrypted text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					stream.open(path + ".txt", ios::out);
				}
				stream << j;
				stream.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Wrong pair alph-key\n";
				continue;
			}
		}
	}

	void decrypt()override//+
	{
		bool good;
		while (true)
		{
			good = true;
			Encrypted_text<string> text(this->type, finder<string>("txt", "TEXT"));
			Alph_Key<string> pair(finder<string>("alph", "ALPH"), finder<string>("key", "KEY"));
			if (pair.ALPH.size() != pair.KEY.size())
			{
				cout << "Wrong pair ahabet-key\n";
				continue;
			}
			try
			{
				ofstream out;
				exception e;
				string decrypted;
				for (int i = 0; i < text.text.length(); i++)
				{
					if(pair.KEY.find(text.text[i])==-1)
					{
						good = false;
						cout << "Invalid symbols found!\n";
						break;
					}
					decrypted = decrypted + pair.ALPH[pair.KEY.find(text.text[i])];
				}
				if (!good)continue;
				string path;
				cout << "Input filename to save decrypted text\n";
				cin >> path;
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Invalid symbols found.Try to input correct symbols!\n";
					continue;
				}
				out.open(path + ".txt");
				while(!out.is_open())
				{
					cout << "Couldn`t open or create the file" << endl;
					cout << "Input filename to save decrypted text\n";
					cin >> path;
					if (!cin.good())
					{
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Invalid symbols found.Try to input correct symbols!\n";
						continue;
					}
					out.open(path + ".txt");
				}
				out << decrypted;
				out.close();
				cout << "Done\n";
				return;
			}
			catch (exception e)
			{
				cout << "Invalid input" << endl;
				continue;
			}
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
		if (!cin.good())
		{
			if (!cin.good())
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid symbols found.Try to input correct symbols!\n";
				continue;
			}
		}
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
		if (!cin.good())
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Invalid symbols found.Try to input correct symbols!\n";
			continue;
		}
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