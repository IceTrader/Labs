#include <iostream>
#include<Windows.h>
#include<string>
#include<fstream>
#include<vector>
#include<thread>
#include<atomic>
#include<mutex>

using namespace std;

class Cracker
{
public:
	enum FLAGS { ALWAYS_YES, ALWAYS_NO, DEFAULT };
	Cracker() = delete;

	Cracker(const string& text_file, int threads = thread::hardware_concurrency(),unsigned long max_key_length=10)//+
	{
		this->key_counter = 0;
		setlocale(LC_ALL, "Russian");
		string text;
		ifstream in(text_file);
		if (!in.is_open())
		{
			cout << "no file\n";
			return;
		}
		while (!in.eof())
		{
			getline(in, text);
			this->encrypted_text = this->encrypted_text + text;
		}
		in.close();
		if (encrypted_text.size() == 0)
		{
			cout << "File is empty!\n";
			return;
		}
		this->denominators = this->find_denoms(this->encrypted_text.size());
		system("Cracking.mp3");
		for(int i=0;i<this->denominators.size();i++)
		{
			vector<vector<char>>table;
			for(int j = 0;j<this->denominators[i];j++)
			{
				vector<char> v;
				for (int k = 0; k < this->encrypted_text.size() / denominators[i]; k++)
					v.push_back(encrypted_text[j * encrypted_text.size() / denominators[i] + k]);
				table.push_back(v);
			}
			if(table[0].size()<=max_key_length)
				this->Crack_alg(table,threads);
		}
	}

	~Cracker() 
	{

	}

	void add_bigrammas(const vector<string>& v)//+
	{
		for (auto& a : v)
			this->bigrammas.push_back(a);
	}

	void add_bigrammas(const string& v)//+
	{
		this->bigrammas.push_back(v);
	}

	bool finder(char c1, char c2)//+
	{
		string bi;
		bi.push_back(c1);
		bi.push_back(c2);
		for (auto& a : this->bigrammas)
			if (a == bi)return true;
		return false;
	}

	vector<vector<int>> get_keys()//+
	{
		return this->keys;
	}

	void decrypt(const string& input_file, const string& output_file, const vector<int>& key, const string& dictionary,FLAGS flag = DEFAULT)
	{
		if (key.size() == 0)
		{
			cout << "Ключ пуст\n";
			return;
		}
		fstream in(dictionary,ios::in), out(input_file);
		vector<string>dictionary_v;
		vector<vector<char>>table(key.size());
		string word;
		if(!in.is_open()||!out.is_open())
		{
			"Файл не открывается!\n";
			return;
		}
		do
		{
			getline(in, word);
			dictionary_v.push_back(word);
		} while (!in.eof());
		in.close();
		int iter = 0, i = 0,counter = 0;
		do
		{
			getline(out, word);
			for (int j = 0; j < word.length(); j++, iter = (iter + 1) % table.size())
				table[iter].push_back(word[j]);
			i++;
		} while (!out.eof() && i < 10);
		out.close();
		for (i = 0; i < table.size(); i++)
			if (table[i].size() > table.back().size())
				table[i].pop_back();
		word.clear();
		for (i = 0; i < table.front().size(); i++)
			for (iter = 0; iter < key.size(); iter++)
			{
				word.push_back(table[key[iter]][i]);
				cout << table[key[iter]][i];
			}
		iter = 0;
		for (i = 0; i < dictionary_v.size(); i++)
		{
			while (true)
			{
				iter = word.find(dictionary_v[i],iter);
				if (iter != string::npos)
				{
					counter++;
				}
				else break;
			}
		}
		cout <<"\nНайдено "<<counter<<" совпадений со словарём.\nПродолжить расшифровывать данным ключом?(Y/N)\n";
		while (true)
		{
			char c;
			try
			{
				if(flag==DEFAULT)
					cin >> c;
				else
				{
					if (flag == ALWAYS_YES)c = 'Y';
					else c = 'N';
				}
				if (c == 'Y')
				{
					in.open(input_file,ios::in);
					table.clear();
					table.resize(key.size());
					iter = i = 0;
					do
					{
						getline(in, word);
						for (int j = 0; j < word.length(); j++, iter = (iter + 1) % table.size())
							table[iter].push_back(word[j]);
						i++;
					} while (!in.eof());
					in.close();
					out.open(output_file, ios::app);
					for (auto& a : key)
						out << a << ' ';
					out << '\n';
					for (i = 0; i < table.front().size(); i++)
					{
						for (iter = 0; iter < key.size(); iter++)
							out << table[key[iter]][i];
					}
					out << '\n';
					out.close();
					cout << "Расшифровано\n";
					break;
				}
				else if (c == 'N')
				{
					cout << "Отмена дешифровки\n";
					return;
				}
				else
					throw exception();
			}
			catch (const exception&)
			{
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Введен другой символ!\n";
			}
		}
	}
private:
	double key_counter;

	void recursive_finder(const vector<vector<char>>& bi_table, vector<int>key)//+
	{
		if(key.size()!=bi_table.size())
		{
			for (int i = 0; i < bi_table.size();i++)
			{
				if(key.empty())
					recursive_finder(bi_table, vector<int>{i});
				else 
				{
					bool flag= false;
					for (int j = 0; j < key.size(); j++)
						if (key[j] == i)flag = true;
					if (flag)continue;
					vector<int> v = key;
					v.push_back(i);
					recursive_finder(bi_table,v);
				}
			}
		}
		else
		{
			this->key_counter++;
			cout << "Проверяю " << this->key_counter << " ключ." << endl;
			for (int i = 0; i < key.size() - 1; i++)
				if (bi_table[key[i]][key[i + 1]] != '0')
					return;
			this->keys.push_back(key);
		}
	}

	vector<string> bigrammas = { "аъ","аь", "бй", "бф","гщ", "гъ", "еъ", "еь", "жй" ,"жц","жщ","жъ" ,
		"жы", "йъ", "къ","лъ","мъ","оъ","пъ","ръ", "уъ", "уь" ,"фщ","фъ", "хы", "хь", "цщ","цъ","цю",
		"чф","чц","чщ","чъ", "чы" ,"чю", "шщ","шъ", "шы","шю","щг","щж","щл", "щх","щц","щч","щш","щъ",
		"щы","щю","щя","ъа","ъб","ъг", "ъд","ъз","ъй", "ък","ъл","ън","ъо","ъп","ър","ъс", "ът","ъу",
		"ъф","ъх","ъц","ъч","ъш","ъщ","ъъ","ъы","ъь","ъэ","ыъ","ыь","ьъ","ьы","эа","эж","эи","эо",
		"эу","эщ","эъ","эы","эь","эю","эя","юъ","юы","юь","яъ","яы","яь","ьь"," ъ"," ь" };

	vector<int> find_denoms(int number)//+
	{
		if (number == 1)
			return vector<int>{1};
		int less;
		for(int i = 2;;i++)
			if (number%i==0)
			{
				less = i;
				break;
			}
		vector<int>v = {1};
		for (int i = less; i < number / less + 1; i++)
			if (number % i == 0) v.push_back(i);
		return v;
	}

	vector<vector<int>>keys;

	vector<int>denominators;

	string encrypted_text;

	void Crack_alg(const vector<vector<char>>& table,const int threads)//+
	{
		vector<vector<char>>bigramma_table;
		for (int i = 0; i < table[0].size(); i++)
		{
			vector<char>v;
			for (int j = 0; j < table[0].size(); j++)
				v.push_back(atomic<char>(' '));
			v[i] = 'X';
			bigramma_table.push_back(v);
		}
		vector<thread>v;
		mutex mtx;
		for (int i = 0; i < threads; i++)
			v.push_back(thread([&table,&bigramma_table,&mtx,this]()
				{
					for(int i=0;i<bigramma_table.size();i++)
					{
						for (int j = 0; j < bigramma_table[i].size(); j++)
						{
							unique_lock<mutex>LOCK(mtx);
							if (bigramma_table[i][j] == ' ')
							{
								bigramma_table[i][j] = '0';
								LOCK.unlock();
								for(int k = 0;k<table.size();k++)
									if(finder(table[k][i], table[k][j]))
									{
										bigramma_table[i][j] = 'X';
										break;
									}
							}
						}
					}
				}));
		for (auto& a : v)
			a.join();
		this->key_counter = 0;
		recursive_finder(bigramma_table,vector<int>());	
	}
};

int main()
{
	Cracker c1("d:kek/e.txt", 3,6);
	for(int i =0;i<c1.get_keys().size();i++)
		c1.decrypt("d:kek/e.txt","d:kek/text1.txt",c1.get_keys()[i], "d:kek/text.txt",Cracker::ALWAYS_YES);
	return 0;
}