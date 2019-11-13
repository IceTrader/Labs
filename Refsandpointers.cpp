#include <iostream>
#include <conio.h>

using namespace std;

void fillarr(int *p,const int &refy)
{
	for(int i=0;i<refy;i++)
		cin>>p[i];
}
void printarr(int *point, const int& refy)
{
	for (int i = 0; i < refy; i++)
		cout<<point[i]<<" ";
	cout <<endl;
}
int main()
{
	int x, y;
	cin >> x >> y;
	int ** mas = new int*[x];
	system("Pause");
	for (int i = 0; i < x; i++)
		mas[i] = new int[y];
	system("Pause");
	for (int i = 0; i < x; i++)
		fillarr(mas[i],y);
	system("Pause");
	for (int i = 0; i < x; i++)
		printarr(mas[i], y);
	system("Pause");
	for (int i = 0; i < x; i++)
		delete[] mas[i];
	system("Pause");
	delete[] mas;
	system("Pause");
}