#pragma once
#include <vector>

template <class Element>
class ObjectPool
{

private:
	//size_t			  activeElements;
	std::vector<Element> elements;

public:
	ObjectPool(size_t maxElements)
	{
		elements.reserve(maxElements);
	}

	size_t capacity() const
	{
		return elements.capacity();
	}

	size_t size() const
	{
		return elements.size();
	}

	Element& at(size_t index)
	{
		return elements.at(index);
	}

	const Element& at(size_t index) const
	{
		return elements.at(index);
	}

	Element& back()
	{
		return elements.back();
	}

	const Element& back() const
	{
		return elements.back();
	}

	template< class... Args >
	void addBackElement(Args&&... args) //todo: rename as emplace_back
	{
		if (this->size() < this->capacity()) {
			elements.emplace_back(std::forward<Args>(args)...);
		}
		else
		{
			throw std::out_of_range("System: maximum limit of elements in the system exceeded.");
		}
	}

	void deleteElement(size_t index) //TODO: rename as erase
	{
		if (index >= this->size()) {
			throw std::out_of_range("System: element requested is not active.");
		}
		swapElements(elements.size() - 1, index);
		elements.pop_back();
	}

	void reserve(size_t maxElements)
	{
		elements.reserve(maxElements);
	}

private:
	void swapElements(size_t i, size_t j)
	{
		using std::swap;
		swap(elements.at(i), elements.at(j));
	}
};

