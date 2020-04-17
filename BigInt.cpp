#include <iostream>
#include<string>
#include <chrono>
#include <ctime>
#include<thread>

using namespace std;

string addition(string obj1, string obj2)
{
	obj1 = "0" + obj1;
	for (int i = obj2.length(); i < obj1.length(); i++)
		obj2 = "0" + obj2;
	string result;
	int boof = 0;
	for (int i = obj1.length() - 1; i > -1; i--)
	{
		result = (char)((boof + obj1[i] + obj2[i] - 96) % 10 + 48) + result;
		(boof + obj1[i] + obj2[i] - 96) / 10 == 1 ? boof = 1 : boof = 0;
	}
	if (result[0] == '0')
		return result.substr(1, result.length());
	return result;
}

string subtraction(string obj1, string obj2)
{
	string result;
	int boof = 0;
	for (int i = obj1.length() - 1; i > -1; i--)
	{
		if (boof + obj1[i] - obj2[i] < 0)
		{
			boof = 10 + obj1[i] - obj2[i];
			obj1[i - 1] = obj1[i - 1] - 1;
			result = (char)(boof + 48) + result;
		}
		else
		{
			result = (char)((boof + obj1[i] - obj2[i]) % 10 + 48) + result;
		}
		boof /= 10;
	}
	for (int i = 0; i < obj1.length() - 1; i++)
		if (result[0] == '0')
			result = result.substr(1, result.length());
	return result;
}

string multiplication(string obj1, string obj2)
{
	if (obj1 == "0" || obj2 == "0")
		return "0";
	string result, boof;
	for (int i = 0; i < obj2.length(); i++)
	{
		boof = "0";
		for (int j = 0; j < obj2[obj2.length() - i - 1] - 48; j++)
		{
			boof.length() >= obj1.length() ? boof = addition(boof, obj1) : boof = addition(obj1, boof);
		}
		for (int j = 0; j < i; j++)
			boof = boof + "0";
		result.length() >= boof.length() ? result = addition(result, boof) : result = addition(boof, result);
	}
	return result;
}

string dividion(string obj1, string obj2)
{
	string result, part;
	if (obj1.length() == obj2.length() && obj2 > obj1 || obj2.length() > obj1.length())
		return "0";
	int first = obj2.length(), last = obj1.length();
	for (int i = first; i <= last; i++)
	{
		string boof = "0";
		part = obj1.substr(0, i);
		while (true)
		{
			if (part[0] == '0')
				part = part.substr(1, part.length());
			else
				break;
		}
		int j = 0;
		while (1)
		{
			if (boof.length() <= part.length())
			{
				if (boof.length() == part.length())
				{
					if (part >= boof)
						boof = addition(obj2, boof);
					else
						break;
				}
				else
					boof = addition(obj2, boof);
			}
			else
			{
				if (j == 0)
				{
					boof = addition(obj2, boof);
					j++;
				}
				break;
			}
			j++;
		}
		j--;
		for (int k = first; k < i; k++)
			boof = "0" + boof;
		for (int i = obj2.length(); i < boof.length(); i++)
			obj2 = "0" + obj2;
		boof = subtraction(boof, obj2);
		for (int k = last; k > i; k--)
			boof = boof + "0";
		while (boof.length() != obj1.length())
		{
			boof = "0" + boof;
		}

		while (true)
		{
			if (obj2[0] == '0')
				obj2 = obj2.substr(1, obj2.length());
			else
				break;
		}
		result = result + char(j + 48);
		obj1 = subtraction(obj1, boof);
		for (int k = obj1.length(); k < last; k++)
			obj1 = "0" + obj1;
	}
	while (1)
	{
		if (result[0] == '0')
			result = result.substr(1, result.length());
		else
			break;
	}
	return result;
}

class BigInt
{
public:
	BigInt()//конструктор по умолчанию
	{
		this->sign = false;
		this->value = "";
	}

	BigInt(bool sign, string value)//конструктор для запланированного создания
	{
		this->sign = sign;
		this->value = value;
	}

	friend istream& operator>>(istream& input, BigInt& big_int)//перегрузка  >>
	{
		string boof;
		input >> boof;
		try
		{
			exception e;//предполагаемое исключение
			for (int i = 0; i < boof.length(); i++)
				if (!((boof[i] >= '0' && boof[i] <= '9') || (i == 0 && boof[0] == '-')))// проверка  на правильность ввода
					throw e;
			if (boof.length() == 1 && boof[0] == '-')
				throw e;
			if (!input.good())
				throw e;
			if (boof[0] == '-')//разбор строки на знак и значение
			{
				big_int.sign = true;
				big_int.value = boof.substr(1, boof.length());
			}
			else
			{
				big_int.sign = false;
				big_int.value = boof;
			}
			return input;
		}
		catch (exception e)// обработка исключения
		{
			cout << "Invalid number!";
			abort();
		}
	}

	friend ostream& operator<<(ostream& output, BigInt big_int)// перегрузка <<
	{
		if (big_int.sign)
			output << "-";
		output << big_int.value;
		return output;
	}

	friend bool operator>(BigInt obj1, BigInt obj2)//перегрузка >
	{
		if (obj1.sign != obj2.sign)
			if (obj1.sign == false)
				return true;
			else
				return false;
		if (obj1.value.length() >= obj2.value.length())
		{
			obj1.value = "0" + obj1.value;
			for (int i = obj2.value.length(); i < obj1.value.length(); i++)
				obj2.value = "0" + obj2.value;
		}
		else
		{
			obj2.value = "0" + obj2.value;
			for (int i = obj1.value.length(); i < obj2.value.length(); i++)
				obj1.value = "0" + obj1.value;
		}
		if (obj1.value > obj2.value && !obj1.sign || obj1.value < obj2.value && obj1.sign)
			return true;
		return false;
	}

	friend bool operator<(BigInt obj1, BigInt obj2)
	{
		if (obj1.sign != obj2.sign)
			if (obj1.sign == false)
				return false;
			else
				return true;
		if (obj1.value.length() >= obj2.value.length())
		{
			obj1.value = "0" + obj1.value;
			for (int i = obj2.value.length(); i < obj1.value.length(); i++)
				obj2.value = "0" + obj2.value;
		}
		else
		{
			obj2.value = "0" + obj2.value;
			for (int i = obj1.value.length(); i < obj2.value.length(); i++)
				obj1.value = "0" + obj1.value;
		}
		if (obj1.value > obj2.value && !obj1.sign || obj1.value < obj2.value && obj1.sign)
			return false;
		return true;
	}

	friend bool operator<=(BigInt obj1, BigInt obj2)//перегрузка <=
	{
		return !(obj1 > obj2);
	}

	friend bool operator>=(BigInt obj1, BigInt obj2)
	{
		return !(obj1 < obj2);
	}

	friend bool operator==(BigInt obj1, BigInt obj2)
	{
		if (obj1.sign != obj2.sign)
			return false;
		if (obj1.value.length() >= obj2.value.length())
		{
			obj1.value = "0" + obj1.value;
			for (int i = obj2.value.length(); i < obj1.value.length(); i++)
				obj2.value = "0" + obj2.value;
		}
		else
		{
			obj2.value = "0" + obj2.value;
			for (int i = obj1.value.length(); i < obj2.value.length(); i++)
				obj1.value = "0" + obj1.value;
		}
		if (obj1.value == obj2.value)
			return true;
		return false;
	}

	friend BigInt operator+(BigInt obj1, BigInt obj2)// перегрузка +
	{
		BigInt result;
		if (obj1.sign == obj2.sign)
		{
			result.sign = obj1.sign;
			obj1.value.length() >= obj2.value.length() ?
				result.value = addition(obj1.value, obj2.value) :
				result.value = addition(obj2.value, obj1.value);
		}
		else
		{
			if (obj1.value.length() >= obj2.value.length())
			{
				obj1.value = "0" + obj1.value;
				for (int i = obj2.value.length(); i < obj1.value.length(); i++)
					obj2.value = "0" + obj2.value;
			}
			else
			{
				obj2.value = "0" + obj2.value;
				for (int i = obj1.value.length(); i < obj2.value.length(); i++)
					obj1.value = "0" + obj1.value;
			}
			if (obj1.value > obj2.value)
			{
				result.sign = obj1.sign;
				result.value = subtraction(obj1.value, obj2.value);
			}
			else
			{
				result.sign = obj2.sign;
				result.value = subtraction(obj2.value, obj1.value);
			}
			if (result.value == "0")
				result.sign = false;
		}
		return result;
	}

	friend BigInt operator-(BigInt obj1, BigInt obj2)
	{
		BigInt result;
		if (obj1.sign != obj2.sign)
		{
			result.sign = obj1.sign;
			obj1.value.length() >= obj2.value.length() ?
				result.value = addition(obj1.value, obj2.value) :
				result.value = addition(obj2.value, obj1.value);
		}
		else
		{
			if (obj1.value.length() >= obj2.value.length())
			{
				obj1.value = "0" + obj1.value;
				for (int i = obj2.value.length(); i < obj1.value.length(); i++)
					obj2.value = "0" + obj2.value;
			}
			else
			{
				obj2.value = "0" + obj2.value;
				for (int i = obj1.value.length(); i < obj2.value.length(); i++)
					obj1.value = "0" + obj1.value;
			}
			if (obj1.value > obj2.value)
			{
				result.sign = false;
				result.value = subtraction(obj1.value, obj2.value);
			}
			else
			{
				result.sign = true;
				result.value = subtraction(obj2.value, obj1.value);
			}
			if (result.value == "0")
				result.sign = false;
		}
		return result;
	}

	BigInt operator++()//перегрузка префиксного ++
	{
		BigInt increment(false, "1");
		*this = *this + increment;
		return *this;
	}

	BigInt operator++(int value)//перегрузка постфиксного ++
	{
		BigInt current = *this;
		BigInt increment(false, "1");
		*this = *this + increment;
		return current;
	}

	BigInt operator--()//перегрузка префиксного --
	{
		BigInt increment(false, "1");
		*this = *this - increment;
		return *this;
	}

	BigInt operator--(int value)//перегрузка постфиксного --
	{
		BigInt current = *this;
		BigInt increment(false, "1");
		*this = *this - increment;
		return current;
	}

	friend BigInt operator*(BigInt& obj1, BigInt& obj2)//перегрузка *
	{
		BigInt result(false, "0");
		result.sign = obj1.sign ^ obj2.sign;
		obj1 > obj2 ?
			result.value = multiplication(obj1.value, obj2.value) :
			result.value = multiplication(obj2.value, obj1.value);
		if (result.value == "0")
			result.sign = false;
		return result;
	}

	friend BigInt operator/(BigInt& obj1, BigInt& obj2)//перегрузка /
	{
		BigInt result(false, "0");
		if (obj2.value == result.value)
		{
			cout << "Can`t be divided by zero!";
			abort();
		}
		if (obj1.value == result.value)
			return result;
		result.sign = obj1.sign ^ obj2.sign;
		result.value = dividion(obj1.value, obj2.value);
		if (result.value == "0")
			result.sign = false;
		return result;
	}

private:
	bool sign;// поле знака
	string value;//поле значения
};

void run(BigInt a, const char c, BigInt b, int& result)
{
	switch (c)
	{
	case '+':
	{
		chrono::time_point<chrono::high_resolution_clock> start, end;
		start = chrono::high_resolution_clock::now();
		for (int i = 0; i < 100000; i++)
			a + b;
		end = chrono::high_resolution_clock::now();
		result = chrono::duration_cast<chrono::seconds>(end - start).count();
		break;
	}
	case '-':
	{
		chrono::time_point<chrono::high_resolution_clock> start, end;
		start = chrono::high_resolution_clock::now();
		for (int i = 0; i < 100000; i++)
			a - b;
		end = chrono::high_resolution_clock::now();
		result = chrono::duration_cast<chrono::seconds>(end - start).count();
		break;
	}
	case '*':
	{
		chrono::time_point<chrono::high_resolution_clock> start, end;
		start = chrono::high_resolution_clock::now();
		for (int i = 0; i < 100000; i++)
			a* b;
		end = chrono::high_resolution_clock::now();
		result = chrono::duration_cast<chrono::seconds>(end - start).count();
		break;
	}
	case '/':
	{
		chrono::time_point<chrono::high_resolution_clock> start, end;
		start = chrono::high_resolution_clock::now();
		for (int i = 0; i < 100000; i++)
			a / b;
		end = chrono::high_resolution_clock::now();
		result = chrono::duration_cast<chrono::seconds>(end - start).count();
		break;
	}
	}
}

void quality_performance_rating(BigInt a, BigInt b)
{
	chrono::time_point<chrono::high_resolution_clock> start, end;
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		a + b;
	}
	end = chrono::high_resolution_clock::now();
	int add_time = chrono::duration_cast<chrono::seconds>(end - start).count();
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		a - b;
	}
	end = chrono::high_resolution_clock::now();
	int sub_time = chrono::duration_cast<chrono::seconds>(end - start).count();
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		a* b;
	}
	end = chrono::high_resolution_clock::now();
	int mull_time = chrono::duration_cast<chrono::seconds>(end - start).count();
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
	{
		a / b;
	}
	end = chrono::high_resolution_clock::now();
	int div_time = chrono::duration_cast<chrono::seconds>(end - start).count();
	cout << "Average time of addition for your big a and b is " << add_time / 100.0 << " ms" << endl
		<< "Average time of subtraction for your big a and b is " << sub_time / 100.0 << " ms" << endl
		<< "Average time of multiplication for your big a and b is " << mull_time / 100.0 << " ms" << endl
		<< "Average time of dividion for your big a and b is " << div_time / 100.0 << " ms" << endl;
}

void fast_performance_rating(BigInt a, BigInt b)
{
	int add_time, sub_time, mull_time, div_time;
	thread add(run, a, '+', b, ref(add_time));
	thread sub(run, a, '-', b, ref(sub_time));

	add.join();
	sub.join();
	thread mull(run, a, '*', b, ref(mull_time));
	thread div(run, a, '/', b, ref(div_time));
	mull.join();
	div.join();
	cout << "Average time of addition for your big a and b is " << add_time / 100.0 << " ms" << endl
		<< "Average time of subtraction for your big a and b is " << sub_time / 100.0 << " ms" << endl
		<< "Average time of multiplication for your big a and b is " << mull_time / 100.0 << " ms" << endl
		<< "Average time of dividion for your big a and b is " << div_time / 100.0 << " ms" << endl;
}

int main()
{
	BigInt a;
	BigInt b;
	while (true)
	{
		cin >> a >> b;
		cout << a * b << endl;
	}

	//fast_performance_rating(a,b);
	//quality_performance_rating(a, b);
	return 0;
}