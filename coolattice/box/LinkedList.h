#pragma once

template <class Element> class LinkedList
{
public:
	Element* next;
	Element* prev;

public:
	LinkedList() : next(nullptr), prev(nullptr) {}

	void insert(Element* newOne)
	{
		newOne->prev = this;
		newOne->next = this->next;
		if (this->next != nullptr)
		{
			this->next->prev = newOne;
		}
		this->next = newOne;
	}

	void remove()
	{
		if (next != nullptr)
			next->prev = prev;
		if (prev != nullptr)
			prev->next = next;
		next = prev = nullptr;
	}

	void clear()
	{
		next = prev = nullptr;
	}

};