#pragma once

#include <vector>



template <class Element>

class ObjectPool

{



private:

	size_t			  activeElements;

	std::vector<Element> elements;



public:

	ObjectPool(size_t maxElements)

	{

		activeElements = 0;

		elements.resize(maxElements);

	}



	size_t size() const

	{

		return activeElements;

	}



	Element& at(size_t index)

	{

		if (index >= activeElements) {

			throw std::out_of_range("System: element requested is not active.");

		}

		return elements.at(index);

	}



	const Element& at(size_t index) const

	{

		if (index >= activeElements) {

			throw std::out_of_range("System: element requested is not active.");

		}

		return elements.at(index);

	}



	Element& back()

	{

		if (activeElements <= 0) {

			throw std::out_of_range("System: number of active elements is zero.");

		}

		return elements.at((size_t)(activeElements - 1));

	}



	const Element& back() const

	{

		if (activeElements <= 0) {

			throw std::out_of_range("System: number of active elements is zero.");

		}

		return elements.at((size_t)(activeElements - 1));

	}



	void addBackElement()

	{

		if (activeElements < elements.size()) {

			activeElements++;

		}

		else

		{

			throw std::out_of_range("System: maximum limit of elements in the system exceeded.");

		}

	}



	void deleteElement(size_t index)

	{

		if (index >= activeElements) {

			throw std::out_of_range("System: element requested is not active.");

		}

		activeElements--;

		swapElements(activeElements, index);

	}



	void resize(size_t maxElements)

	{

		elements.resize(maxElements);

	}



private:

	void swapElements(size_t i, size_t j)

	{

		std::swap(elements.at(i), elements.at(j));

	}

};