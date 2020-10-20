#pragma once
#include <iostream>
#include<string>
#include <stdexcept>

using namespace std;
template<typename T>
class MyIterator
{
protected:
	T* that;
public:
	MyIterator(T* that)
	{
		this->that = that;
	}
	MyIterator(const MyIterator<T>& i)
	{
		this->that = *i;
	}
	virtual T& operator+(int index)
	{
		return *(that + index);
	}
	virtual T& operator-(int index)
	{
		return *(that - index);
	}
	virtual T& operator++(int index)
	{
		return *that++;
	}
	virtual T& operator++()
	{
		return *++that;
	}
	virtual T& operator--(int index)
	{
		return *that--;
	}
	virtual T& operator--()
	{
		return *--that;
	}
	virtual T& operator*()
	{
		return *that;
	}
	bool operator==(const MyIterator& iter)
	{
		return that == iter.that;
	}
	bool operator!=(const MyIterator& iter)
	{
		return that != iter.that;
	}
};

template<typename T>
class MyArray
{
	T* first;
	T* last;
	int size;
public:
	MyArray() = delete;
	MyArray(const int size)
	{
		this->size = size;
		this->first = new T[size];
		this->last = this->first + size - 1;
	}
	~MyArray()
	{
		delete[] this->first;
	}
	T& operator[](const unsigned int index)
	{
		try
		{
			for (int i = 0; i < this->size; i++)
				if (i == index)return *(this->first + index);
			throw new exception();
		}
		catch (const std::exception&)
		{
			cout << "Out of bounds\n";
		}
	}
	MyIterator<T> begin()
	{
		return MyIterator<T>(this->first);
	}
	MyIterator<T> end()
	{
		return MyIterator<T>(this->last);
	}
};


int main()
{
	MyArray<int>arr(10);
	for (int i = 0; i < 10; i++)
	{
		arr[i] = i;
		cout << arr[i] << endl;
	}
	MyIterator<int>Iter(arr.begin());
	cout << Iter + 1 << endl;
return 0;
}