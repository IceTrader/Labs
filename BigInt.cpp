#include <iostream>
#include<string>

using namespace std;

class BigInt
{
private: 
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

public:
	BigInt()//конструктор по умолчанию
	{
		this->sign = false;
		this->value = "";
		this->boof = "";
	}

	BigInt(bool sign, string value)//конструктор для запланированного создания
	{
		this->sign = sign;
		this->value = value;
		this->boof = "";
	}

	~BigInt()// деструктор?
	{

	}

	friend istream& operator>>(istream& input,BigInt&big_int)//перегрузка  >>
	{
		input >> big_int.boof;
		try
		{
			exception e;//предполагаемое исключение
			for (int i = 0; i <big_int.boof.length(); i++)
				if (!((big_int.boof[i] >= '0' && big_int.boof[i] <= '9') || (i == 0&& big_int.boof[0] == '-')))// проверка  на правильность ввода
					throw e;
			if (big_int.boof.length() == 1 && big_int.boof[0] == '-')
				throw e;
			if (!input.good())
				throw e;
			if (big_int.boof[0] == '-')//разбор строки на знак и значение
			{
				big_int.sign = true;
				big_int.value = big_int.boof.substr(1, big_int.boof.length());
			}
			else
				big_int.value = big_int.boof;
			big_int.boof="";
			return input;
		}
		catch(exception e)// обработка исключения
		{
			cout << "Invalid number!";
			abort();
		}
	}

	friend ostream& operator<<(ostream& output, BigInt& big_int)// перегрузка <<
	{
		if (big_int.sign)
			big_int.boof = "-" + big_int.value;
		else
			big_int.boof = big_int.value;
		output << big_int.boof;
		big_int.boof = "";
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
		if (obj1.value > obj2.value&&!obj1.sign|| obj1.value < obj2.value && obj1.sign)
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

	friend BigInt& operator+(BigInt obj1,BigInt obj2)// перегрузка +
	{
		BigInt* result=new BigInt();
		if(obj1.sign==obj2.sign)
		{
			result->sign = obj1.sign;
			obj1.value.length() >= obj2.value.length() ?
									result->value = result->addition(obj1.value, obj2.value) : 
													result->value =result->addition(obj2.value,obj1.value);
		}
		else
		{
			if(obj1.value.length()>=obj2.value.length())
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
			if(obj1.value>obj2.value)
			{
				result->sign = obj1.sign;
				result->value = result->subtraction(obj1.value, obj2.value);
			}
			else
			{
				result->sign = obj2.sign;
				result->value = result->subtraction(obj2.value, obj1.value);
			}
			if (result->value == "0")
				result->sign = false;
		}
		return *result;
	}

	friend BigInt& operator-(BigInt obj1,BigInt obj2)
	{
		BigInt* result = new BigInt();
		if (obj1.sign != obj2.sign)
		{
			result->sign = obj1.sign;
			obj1.value.length() >= obj2.value.length() ?
									result->value = result->addition(obj1.value, obj2.value) :
													result->value = result->addition(obj2.value, obj1.value);
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
				result->sign = false;
				result->value = result->subtraction(obj1.value, obj2.value);
			}
			else
			{
				result->sign = true;
				result->value = result->subtraction(obj2.value, obj1.value);
			}
			if (result->value == "0")
				result->sign = false;
		}
		return *result;
	}

	BigInt& operator++()//перегрузка префиксного ++
	{
		BigInt increment(false, "1");
		*this = *this + increment;
		return *this;
	}

	BigInt& operator++(int value)//перегрузка постфиксного ++
	{
		BigInt *current=new BigInt(this->sign,this->value);
		BigInt increment(false,"1");
		*this = *this + increment;
		return *current;
	}

	BigInt& operator--()//перегрузка префиксного --
	{
		BigInt increment(false, "1");
		*this = *this - increment;
		return *this;
	}

	BigInt& operator--(int value)//перегрузка постфиксного --
	{
		BigInt* current = new BigInt(this->sign, this->value);
		BigInt increment(false, "1");
		*this = *this - increment;
		return *current;
	}

	friend BigInt& operator*(BigInt obj1, BigInt obj2)//перегрузка *
	{
		BigInt* result = new BigInt(false,"0");
		result->sign = obj1.sign ^ obj2.sign;
		if (obj1 > obj2)
		{
			obj2.sign = false;
			obj1.sign = result->sign;
			for (BigInt i(false, "1"); i < obj2; i++)
				*result = *result + obj1;
		}
		else
		{
			obj1.sign = false;
			obj2.sign = result->sign;
			for (BigInt i(false, "1"); i < obj1; i++)
				*result = *result + obj2;
		}
		return *result;
	}

	friend BigInt& operator/(BigInt obj1, BigInt obj2)//перегрузка /
	{
		BigInt* result = new BigInt(false, "0");
		if (obj2.value == result->value)
		{
			cout << "Can`t be divided by zero!";
			abort();
		}
		result->sign = obj1.sign ^ obj2.sign;
			obj1.sign = false;
			obj2.sign = false;
			BigInt i(false, "0");
			for(;obj1>i;obj1=obj1-obj2)
			{
				result->value=result->addition(result->value,"1");
			}
			if(obj1.value != "0")
			{
				bool sign = result->sign;
				result->sign = false;
				--*(result);
				result->sign = sign;
			}
		return *result;
	}

private:
	bool sign;// поле знака
	string value;//поле значения
	string boof;// буфер 
};

int main()
{
	BigInt a;
	BigInt b;
	cin >> a >> b;
	cout << a + b << endl;
	cout << a - b << endl;
	cout << a * b << endl;
	cout << a / b << endl;
	cout << a << endl << b;
	return 0;
}