#include<iostream>
#include<fstream>
#include<windows.h>
#include<string>
#include<vector>
using namespace std;
vector<vector<int>> s()
{
	int sum;
	int a[2][3] = { {1,2,3},{1,2,3} };
	int b[3][2] = { {1,2},{1,2},{1,2} };
	vector<vector<int>>r(2, vector<int>(2));
	cout << "MATRIX a\n\n";
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << a[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "MATRIX b\n\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			cout << b[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "REZULT\n";
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			sum = 0;
			for (int k = 0; k < 3; k++)
			{
				sum = sum + a[i][k] * b[k][j];
			}
			r[i][j] = sum;
			cout << sum << " ";
		}
		cout << "\n";
	}
	return r;
}
vector<vector<int>> l(bool in, vector<vector<int>>A, vector<vector<int>>B)
{
	int sum;
	if (in)
	{
		vector<vector<int>>r(size(A), vector<int>(size(B[0])));
		for (int i = 0; i < size(A); i++)
		{
			for (int j = 0; j < size(B[0]); j++)
			{
				sum = 0;
				for (int k = 0; k < size(B); k++)
				{
					sum = sum + A[i][k] * B[k][j];
				}
				r[i][j] = sum;
				cout << sum << " ";
			}
			cout << "\n";
		}
		return r;
	}
	int x, y, m;
	cout << "VVEDITE RAZMERNOSTI\n";
	cin >> x >> y >> m;
	int** a = new int* [y];
	for (int i = 0; i < y; i++)a[i] = new int[x];
	int** b = new int* [x];
	for (int i = 0; i < x; i++)b[i] = new int[m];
	vector<vector<int>>r(y, vector<int>(m));
	cout << "ZAPOLNIAEM 1 MASSIV\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			cin >> a[i][j];
		}
	}
	cout << "ZAPOLNIAEM 2 MASSIV\n";
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> b[i][j];
		}
	}
	cout << "MATRIX a\n\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			cout << a[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "MATRIX b\n\n";
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << b[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "REZULTIRUEM\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < m; j++)
		{
			sum = 0;
			for (int k = 0; k < x; k++)
			{
				sum = sum + a[i][k] * b[k][j];
			}
			r[i][j] = sum;
			cout << sum << " ";
		}
		cout << "\n";
	}
	for (int i = 0; i < y; i++)
	{
		delete[] a[i];
	}
	delete[] a;
	for (int i = 0; i < x; i++)
	{
		delete[] b[i];
	}
	delete[] b;
	return r;
}
vector<vector<int>> v(bool in, vector<vector<int>>A, vector<vector<int>>B)
{
	int sum;
	if (in)
	{
		vector<vector<int>>r(size(A), vector<int>(size(B[0])));
		for (int i = 0; i < size(A); i++)
		{
			for (int j = 0; j < size(B[0]); j++)
			{
				sum = 0;
				for (int k = 0; k < size(B); k++)
				{
					sum = sum + A[i][k] * B[k][j];
				}
				r[i][j] = sum;
				cout << sum << " ";
			}
			cout << "\n";
		}
		return r;
	}
	int x, y, m;
	cout << "VVEDITE RAZMERNOSTI\n";
	cin >> x >> y >> m;
	vector<vector<int>>a(y, vector<int>(x));
	vector<vector<int>>b(x, vector<int>(m));
	vector<vector<int>>r(y, vector<int>(m));
	cout << "FILLING MATRIX 1\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			cin >> a[i][j];
		}
	}
	cout << "FILLING MATRIX 2\n";
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> b[i][j];
		}
	}
	cout << "MATRIX A\n\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			cout << a[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "MATRIX B\n\n";
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cout << b[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "MULTING\nREZULT\n";
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < m; j++)
		{
			sum = 0;
			for (int k = 0; k < x; k++)
			{
				sum = sum + a[i][k] * b[k][j];
			}
			r[i][j] = sum;
			cout << sum << " ";
		}
		cout << "\n";
	}
	return r;
}
vector<vector<int>> mat(bool toin, vector<vector<int>>A, vector<vector<int>>B)
{
	char sw;
	while (1)
	{
		cout << "s=static example of matrix mult\n" << "l=mult your matrixes with labels\n" << "v=mult your matrixes with vectors\n";
		cin >> sw;
		switch (sw)
		{
		case'l':
		{
			return l(toin, A, B);
		}
		case's':
		{
			if (!toin)
				return s();
			else
			{
				cout << "Couldn`t write from file to static Matrix\n";
				break;
			}
		}
		case'v':
		{
			return v(toin, A, B);
		}
		default:
		{
			cout << "STUPID USER";
			break;
		}
		}
	}
}
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
	string choice, filename, username;
	int e = 0;
	ifstream filein;
	ofstream fileout;
	cout << "Write down your username\n";
	cin >> username;
	if (username == "Admin" || username == "admin" || username == "ADMIN")
		for (int i = 0; i < 100; i++)
		{
			cout << "ÂÇËÎÌ ÆÎÏÛ ";
			Sleep(100);
		}
	cout << endl;
	while (1)
	{
		vector<vector<int>>A;
		vector<vector<int>>B;
		cout << "What do you want to do?\nP.S.  Try to enter the word help to get more information\n";
		cin >> choice;
		if (choice == "NF")
		{
			cout << "Enter your new file name\n";
			cin >> filename;
			fileout.open(filename);
			if (!fileout.is_open())
			{
				cout << "Couldn`t open the file\n";
				break;
			}
			cout << "File opened successfully\n";
			fileout << "{\n";
			for (auto i : mat(0, A, B))
			{
				fileout << " ";
				for (auto j : i)
				{
					fileout << j << " ";
				}
				fileout << endl;
			}
			fileout << "}(By " << username << ")" << endl;
		}
		else if (choice == "EF")
		{
			cout << "Enter the name of file,that you want to edit\n";
			cin >> filename;
			fileout.open(filename, ios::app);
			if (!fileout.is_open())
			{
				cout << "Couldn`t open the file\n";
				break;
			}
			cout << "File opened successfully\n";
			fileout << "{\n";
			for (auto i : mat(0, A, B))
			{
				fileout << " ";
				for (auto j : i)
				{
					fileout << j << " ";
				}
				fileout << endl;
			}
			fileout << "}(By " << username << ")" << endl;
		}
		else if (choice == "RFF")
		{
			cout << "Enter your file name\n";
			cin >> filename;
			filein.open(filename);
			if (!filein.is_open())
			{
				cout << "Couldn`t open the file\n";
				break;
			}
			cout << "File opened successfully\n";
			cout << "Reading data from 1st Matrix\n";
			string s;
			getline(filein, s);
			for (int i = 0; s.find("}") != 0; i++)
			{
				if (filein.eof())
				{
					e++; break;
				}
				vector<int>b;
				getline(filein, s);
				for (int j = 1; j < size(s) && s[j] != '\n'; j++)
				{
					if (s[0] == '}')break;
					if (s[j] == ' ' && s[j - 1] != ' ')
						b.push_back((int)s[j - 1] - 48);
					if (j > 2)
					{
						if (s[j - 1] != ' ' && s[j - 2] != ' ')
						{
							for (int k = j - 2; k > 0 && s[k] != ' '; k--)
								b[size(b) - 1] = b[size(b) - 1] + ((int)s[k] - 48) * 10;
						}
					}
				}
				if (!b.empty())A.push_back(b);
			}
			cout << "Reading data from 2st Matrix\n";
			getline(filein, s);
			for (int i = 0; s.find("}") != 0; i++)
			{
				if (filein.eof())
				{
					e++; break;
				}
				vector<int>b;
				getline(filein, s);
				for (int j = 1; j < size(s) && s[j] != '\n'; j++)
				{
					if (s[0] == '}')break;
					if (s[j] == ' ' && s[j - 1] != ' ')
						b.push_back((int)s[j - 1] - 48);
					if (j > 2)
					{
						if (s[j - 1] != ' ' && s[j - 2] != ' ')
						{
							for (int k = j - 2; k > 0 && s[k] != ' '; k--)
								b[size(b) - 1] = b[size(b) - 1] + ((int)s[k] - 48) * 10;
						}
					}
				}
				if (!b.empty())B.push_back(b);
			}
			if (e != 0)
			{
				cout << "Not enough data\n";
				break;
			}
			cout << "Do you want to write rezult inro this file?\n" << "Y/N\n";
			cin >> choice;
			if (choice == "Y")
			{
				filein.close();
				fileout.open(filename, ios::app);
				fileout << "{\n";
				for (auto i : mat(1, A, B))
				{
					fileout << " ";
					for (auto j : i)
					{
						fileout << j << " ";
					}
					fileout << endl;
				}
				fileout << "}(By " << username << ")" << endl;
			}
			else mat(1, A, B);
		}
		else if (choice == "Exit")
		{
			cout << "Goodbye\n";
			break;
		}
		else if (choice == "Help")
		{
			cout << "Commands with description\n"
				<< "NF=make new file\n"
				<< "EF=edit your file\n"
				<< "RFF=read from file and write to consol\n"
				<< "Exit=to exit\n";
		}
		else
		{
			cout << "Command not found,try again!\n";
		}
		if (fileout.is_open()) fileout.close();
		if (filein.is_open()) filein.close();
	}
	return 0;
}