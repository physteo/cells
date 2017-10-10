#include "GridTraj.h"

void GridTraj::fillGrid(const std::vector<BoxCell>* subBoxes)
{
	for (size_t i = 0; i < subBoxes->size(); i++)
	{
		const BoxCell& subBox = subBoxes->at(i);
		Part* current = subBox.head.next;
		int counter = 0;
		while (current != nullptr)
		{
			counter++;
			current = current->next;
		}

		grid.at(i) = counter;
		leftDownCorner.at(i) = subBox.downLeftCorner;
		rightUpperCorner.at(i) = subBox.upperRightCorner;


	}
}
