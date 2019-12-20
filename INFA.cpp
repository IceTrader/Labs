#include <iostream>
#include<string>
#include<math.h>

using namespace std;

int to_dec(string number,int base)
{
	unsigned int new_number = 0;
	for (int i = 0; i < number.length(); i++) 
		switch (number[i])
		{
			case '0':
				break;
			case '1':
			{
				new_number += pow(base,number.length()- i - 1);
				break;
			}
			case '2':
			{
				new_number += 2 * pow(base, number.length() - i - 1);
				break;
			}
			case '3':
			{
				new_number += 3 * pow(base, number.length() - i - 1);
				break;
			}
			case '4':
			{
				new_number += 4 * pow(base, number.length() - i - 1);
				break;
			}
			case '5':
			{
				new_number += 5 * pow(base, number.length() - i - 1);
				break;
			}
			case '6':
			{
				new_number += 6 * pow(base, number.length() - i - 1);
				break;
			}
			case '7':
			{
				new_number += 7 * pow(base, number.length() - i - 1);
				break;
			}
			case '8':
			{
				new_number += 8 * pow(base, number.length() - i - 1);
				break;
			}
			case '9':
			{
				new_number += 9 * pow(base, number.length() - i - 1);
				break;
			}
			case 'A':
			{
				new_number += 10 * pow(base, number.length() - i - 1);
				break;
			}
			case 'B':
			{
				new_number += 11 * pow(base, number.length() - i - 1);
				break;
			}
			case 'C':
			{
				new_number += 12 * pow(base, number.length() - i - 1);
				break;
			}
			case 'D':
			{
				new_number += 13 * pow(base, number.length() - i - 1);
				break;
			}
			case 'E':
			{
				new_number += 14 * pow(base, number.length() - i - 1);
				break;
			}
			case 'F':
			{
				new_number += 15 * pow(base, number.length() - i - 1);
				break;
			}
			default:
			{
				cout << "Unexpected symbol" << endl;
				abort();
			}
		}
	return new_number;
}

string dec_to_num(int number,int base)
{
	string result;
	do
	{
		switch (number % base)
		{
		case 0:
		{
			result = "0" + result;
			break;
		}
		case 1:
		{
			result = "1" + result;
			break;
		}
		case 2:
		{
			result = "2" + result;
			break;
		}
		case 3:
		{
			result = "3" + result;
			break;
		}
		case 4:
		{
			result = "4" + result;
			break;
		}
		case 5:
		{
			result = "5" + result;
			break;
		}
		case 6:
		{
			result = "6" + result;
			break;
		}
		case 7:
		{
			result = "7" + result;
			break;
		}
		case 8:
		{
			result = "8" + result;
			break;
		}
		case 9:
		{
			result = "9" + result;
			break;
		}
		case 10:
		{
			result = "A" + result;
			break;
		}
		case 11:
		{
			result = "B" + result;
			break;
		}
		case 12:
		{
			result = "C" + result;
			break;
		}
		case 13:
		{
			result = "D" + result;
			break;
		}
		case 14:
		{
			result = "E" + result;
			break;
		}
		case 15:
		{
			result = "F" + result;
			break;
		}
		default:
		{
			cout << "Unexpected symbol" << endl;
			abort();
		}
		}
		number /= base;
	} while (number != 0);
	return result;
}

string base_to_base(string number, int base_in, int base_out)
{
	return dec_to_num(to_dec(number,base_in),base_out);
}

int main()
{
	string x;
	int a, b;
	cin >> x >> a >> b;
	cout << base_to_base(x,a,b) << endl;
	return 0;
}