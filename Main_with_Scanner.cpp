#include"Scanner.h"

int main()
{
	string s, s1;
	Scanner in;
	//cout << "Input path to logs\n";
	//cin >> s;
	while(1)
	{
		cout << "Input device name\n";
		cin >> s1;
		in.search_files("D:/kek/logs", s1);
		//in.search_files("D:/kek/$logs", "PAQ");
	}
	
	return 0;
}