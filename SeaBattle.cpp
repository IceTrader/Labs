#include <iostream>
#include <vector>
#include<ctime>

using namespace std;

bool free(vector<vector<char>>& sea,int x,int y)
{
	if (sea[x][y] == '#')
	{
		return false;
	}
	else if ((x > 1) && (x < 10) && (y > 1) && (y < 10) &&
		(sea[x + 1][y] == '#' || sea[x + 1][y + 1] == '#' || sea[x + 1][y - 1] == '#'
			|| sea[x - 1][y] == '#' || sea[x - 1][y + 1] == '#' || sea[x - 1][y - 1] == '#'
			|| sea[x][y + 1] == '#' || sea[x][y - 1] == '#'))
	{
		return false;
	}
	else if (((x == 1 && y == 1 && (sea[1][2] == '#' || sea[2][1] == '#' || sea[2][2] == '#'))
		|| (x == 1 && y == 10 && (sea[1][9] == '#' || sea[2][10] == '#' || sea[2][9] == '#'))
		|| (x == 10 && y == 1 && (sea[10][2] == '#' || sea[9][1] == '#' || sea[9][2] == '#'))
		|| (x == 10 && y == 10 && (sea[10][9] == '#' || sea[9][10] == '#' || sea[9][9] == '#'))))
	{
		return false;
	}
	else if ((x == 1 && y > 1 && y < 10
		&& (sea[2][y] == '#' || sea[2][y + 1] == '#' || sea[2][y - 1] == '#' || sea[1][y - 1] == '#' || sea[1][y + 1] == '#'))
		|| (x == 10 && y > 1 && y < 10
			&& (sea[9][y] == '#' || sea[9][y + 1] == '#' || sea[9][y - 1] == '#' || sea[10][y - 1] == '#' || sea[10][y + 1] == '#'))
		|| (y == 1 && x > 1 && x < 10
			&& (sea[x][2] == '#' || sea[x - 1][2] == '#' || sea[x + 1][2] == '#' || sea[x + 1][1] == '#' || sea[x - 1][1] == '#'))
		|| (y == 10 && x > 1 && x < 10
			&& (sea[x][9] == '#' || sea[x - 1][9] == '#' || sea[x + 1][9] == '#' || sea[x + 1][10] == '#' || sea[x - 1][10] == '#')))
	{
		return false;
	}
	else
		return true;
}

void prepare(vector<vector<char>> &sea)
{
	for (int i = 0; i < 10; i++)
		sea[i+1][0] = char(i+48);
	for (int i = 'a'; i <'k'; i++)
		sea[0][i - 'a'+1] = char(i);
	for (int i = 1; i < 11; i++)
		for (int j = 1; j < 11; j++)
			sea[i][j] = '-';
}

void show(vector<vector<char>>& sea)
{
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 11; j++)
			cout << sea[i][j] << " ";
		cout << endl;
	}
}

void set_ship(vector<vector<char>>& sea,int size)
{
	while(1)
	{
		int x = 0;
		int y = 0;
		while (sea[x][y] != '#')
		{
			srand(time(0));
			x = rand() % 10 + 1;
			y = rand() % 10 + 1;
			if (free(sea, x, y))
				sea[x][y] = '#';
			else
			{
				x = 0;
				y = 0;
			}
		}
		sea[x][y] = '-';
		bool horizontal = rand() % 2;
		if (horizontal)
		{
			if (x > 5.5)
			{
				int c = 1;
				for (int i = 1; i < size; i++)
					if (free(sea, x - i, y))
						c++;
				if (c == size)
				{
					for (int i = 0; i < size; i++)
						sea[x - i][y] = '#';
					return;
				}
			}
			else
			{
				int c = 1;
				for (int i = 1; i < size; i++)
					if (free(sea, x + i, y))
						c++;
				if (c == size)
				{
					for (int i = 0; i < size; i++)
						sea[x + i][y] = '#';
					return;
				}
			}
		}
		else
		{
			if (y > 5.5)
			{
				int c = 1;
				for (int i = 1; i < size; i++)
					if (free(sea, x, y - i))
						c++;
				if (c == size)
				{
					for (int i = 0; i < size; i++)
						sea[x][y - i] = '#';
					return;
				}
			}
			else
			{
				int c = 1;
				for (int i = 1; i < size; i++)
					if (free(sea, x, y + i))
						c++;
				if (c == size)
				{
					for (int i = 0; i < size; i++)
						sea[x][y + i] = '#';
					return;
				}
			}
		}
	}
}

void fill_ships(vector<vector<char>>& sea)
{
	for (int i = 4; i > 0; i--)
	{
		for (int j = 5 - i; j > 0; j--)
			set_ship(sea,i);
	}
}

int main()
{
	vector<vector<char>>sea(11, vector<char>(11));
	prepare(sea);
	fill_ships(sea);
	show(sea);
	return 0;
}