#pragma once
#include <iostream>
#include <stdexcept>

using namespace std;

template<typename T>
class LinkedList
{
public:
	LinkedList();		//по умолчанию
	~LinkedList();		//деструктор
	void add(T data, int index = 0, string s = "TAIL");		//добавить
	int get_size()
	{
		return Size;
	}								//Вернуть размер
	bool is_empty()
	{
		if (this->Size == 0)
			return true;
		return false;
	}
	T& operator[](const unsigned int index);//Перезрузить скобочки чтобы обращаться по номеру 
	void pop(int index);
	void show();
	void clear(int index = 0);
	void swap(T& e1, T& e2);
	template<typename T>
	class Element
	{
	public:
		T data;
		Element* next_address;
		Element* prev_address;
		Element(T data = T(), Element* prev_address = nullptr, Element* next_address = nullptr)
		{
			this->data = data;
			this->next_address = next_address;
			this->prev_address = prev_address;
		}
	};
	friend bool operator==(const Element<T> e1, const Element<T> e2)
	{
		return (e1.data == e2.data);
	}
protected:
	void behead();// обезглавить:)
	void amputation();
	
	int Size;
	Element<T>* HEAD;
	Element<T>* TAIL;
};

template<typename T>
LinkedList<T>::LinkedList()
{
	Size = 0;
	HEAD = nullptr;
	TAIL = nullptr;
}

template<typename T>
LinkedList<T>::~LinkedList()
{
	clear();
}

template<typename T>
void LinkedList<T>::add(T data, int index, string s)
{
	if (HEAD == nullptr)
	{
		HEAD = new Element<T>(data);
		TAIL = HEAD;
	}
	else
	{
		if (index != get_size() && index != 0 && index < (int)((Size / 2.0) + 0.5))
		{
			Element<T>* current = this->HEAD;
			Element<T>* current_n = current->next_address;
			while (index - 1 != 0)
			{
				current = current->next_address;
				current_n = current->next_address;
				index--;
			}
			current->next_address = new Element<T>(data, current, current_n);
			current_n->prev_address = current->next_address;
		}
		else if (index != get_size() && index != 0)
		{
			Element<T>* current = this->TAIL;
			Element<T>* current_p = current->prev_address;
			while (index + 1 != Size)
			{
				current = current->prev_address;
				current_p = current->prev_address;
				index++;
			}
			current->prev_address = new Element<T>(data, current_p, current);
			current_p->next_address = current->prev_address;
		}
		else if (index == 0 && s == "HEAD")
		{
			HEAD->prev_address = new Element<T>(data, nullptr, HEAD);
			HEAD = HEAD->prev_address;
		}
		else
		{
			TAIL->next_address = new Element<T>(data, TAIL);
			TAIL = TAIL->next_address;
		}
	}
	Size++;
}

template<typename T>
T& LinkedList<T>::operator[](const unsigned int  index)
{
	try
	{
		if (index >= Size)
		{
			cout << "LinkedList::operator[] out of range" << endl;
			throw out_of_range("LinkedList::operator[]");
		}
	}
	catch (out_of_range & e)
	{
		abort();
	}
	int counter;
	if (index < Size / 2)
	{
		counter = 0;
		Element<T>* current = this->HEAD;
		while (index < (int)((Size / 2.0) + 0.5))
		{
			if (counter == index)
				return current->data;
			current = current->next_address;
			counter++;
		}
	}
	else
	{
		counter = Size - 1;
		Element<T>* current = this->TAIL;
		while (index < Size)
		{
			if (counter == index)
				return current->data;
			current = current->prev_address;
			counter--;
		}
	}
}

template<typename T>
void LinkedList<T>::behead()
{
	Element<T>* boof = HEAD;
	HEAD = HEAD->next_address;
	delete boof;
	Size--;
}

template<typename T>
void LinkedList<T>::amputation()
{
	Element<T>* boof = TAIL;
	TAIL = TAIL->prev_address;
	delete boof;
	Size--;
}

template<typename T>
void LinkedList<T>::pop(int index)
{
	if (index + 1 > Size)
	{
		cout << "No such position" << endl;
		return;
	}
	if (index == 0)
		behead();
	else if (index + 1 == Size)
		amputation();
	else
	{
		if (index < (int)((Size / 2.0) + 0.5))
		{
			Element<T>* current = this->HEAD;
			while (index - 1 != 0)
			{
				current = current->next_address;
				index--;
			}
			Element<T>* boof = current->next_address;
			boof->next_address->prev_address = current;
			current->next_address = boof->next_address;
			delete boof;
		}
		else
		{
			Element<T>* current = this->TAIL;
			Element<T>* current_p = current->prev_address;
			while (index + 2 != Size)
			{
				current = current->prev_address;
				current_p = current->prev_address;
				index++;
			}
			Element<T>* boof = current->prev_address;
			boof->prev_address->next_address = current;
			current->prev_address = boof->prev_address;
			delete boof;
		}
		Size--;
	}
}

template<typename T>
void LinkedList<T>::show()
{
	Element<T>* current = this->HEAD;
	if (current == nullptr)
	{
		cout << "List is empty" << endl;
		return;
	}
	while (current != nullptr)
	{
		T boof = current->data;
		cout << typeid(boof).name() << endl;
		current = current->next_address;
	}
}

template<typename T>
void LinkedList<T>::clear(int lenght)
{
	if (lenght == 0)
	{
		while (Size != 0)
			behead();
	}
	else if (lenght < 0)
	{
		while (lenght != 0)
		{
			amputation();
			lenght++;
		}
	}
	else
	{
		while (lenght != 0)
		{
			behead();
			lenght--;
		}
	}
}

template<typename T>
void LinkedList<T>::swap(T& e1, T& e2)
{
	Element<T> buf = e1;
	e1 = e2;
	e2 = buf;
	delete buf;
}