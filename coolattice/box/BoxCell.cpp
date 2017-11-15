#include "BoxCell.h"

BoxCell::BoxCell()
{
	downLeftCorner = Vector{0.0,0.0};
	upperRightCorner = Vector{0.0,0.0};

	for (size_t i = 0; i < 9; i++)
	{
		neighbour.at(i) = nullptr;
	}
	head = Part();
}

void BoxCell::printSubCellList(int subCellNumber) const
{
	Part* current = head.next;
	std::cout << "subBox: " << subCellNumber << " --- ";
	while (current != nullptr)
	{
		std::cout << current->getCell() << "/" << (current->type ? "B" : "F") << "->";
		current = current->next;
	}
	std::cout << std::endl;

}


int  BoxCell::getSubCellListLength() const
{
	Part* current = head.next;
	int length = 0;
	while (current != nullptr)
	{
		length++;
		current = current->next;
	}
	return length;
}

