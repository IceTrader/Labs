#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<filesystem>

using namespace std;

class Scanner
{
	private:
	void go_to_double_points(string& s, int& i)
	{
		while (s[i] != ':' && i < s.length())
			i++;
	}

	void search_logs(string path,const string& object, string name)
	{
		fstream of;
		of.open(path, ios::in);
		while (!of.eof())
		{
			string boof, username, data;
			getline(of, boof);
			if (boof.length() == 0 || boof.find("garbage") != -1)
				continue;
			int iter = 0;
			go_to_double_points(boof, iter);
			data += boof.substr(0, iter);
			boof.replace(0, iter + 1, "");
			iter = 0;
			go_to_double_points(boof, iter);
			username += boof.substr(0, iter);
			if (username != object)
				continue;
			boof.replace(0, iter + 1, "");
			cout << "DATA "<< name.substr(5, name.length() - 10) <<" SECOND "<< data << " INFO " << boof << endl;
		}
		of.close();
	}
public:
	void search_files(string directory_path, string object)
	{
		try
		{
			for (auto& pointer : filesystem::directory_iterator(directory_path))
			{
				auto p = pointer.path().filename().string();
				search_logs(pointer.path().string(), object, p);
			}
		}
		catch (exception e)
		{ 
			cout << "Invalid file name!" << endl;
		}
	}
};