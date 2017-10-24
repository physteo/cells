#include "TractionAnalysis.h"

void TractionAnalysis::analyze(const System* system)
{
	const std::vector<Part*>& parts = system->getParts();
	const CellColony& cells = system->getConstCellColony();
	const Box& box = system->getBox();
	const PartSpecs& partSpecs = system->getPartSpecs();
	const MeasureTwoBodyForce& cilForce = system->getMeasureTwoBodyForce();

	// loop over the subBoxes
	const std::vector<BoxCell> boxCells = box.getBoxCells();
	for (int c = 0; c < boxCells.size(); c++)
	{
		Vector bottomLeftCorner = boxCells.at(c).downLeftCorner;
		Vector upperRightCorner = boxCells.at(c).upperRightCorner;
		Vector centerPosition = (bottomLeftCorner + upperRightCorner) * (0.5);

		for (int J = 0; J < 9; J++)
		{
			const BoxCell* subBox = boxCells.at(c).getNeighbour(J);

			const Part* part = subBox->head.next;
			while (part != nullptr)
			{
				// do smt
				// get Cil force for this particle
				size_t cell = part->cell;
				

				// get friction force

				// compute traction force


				part = part->next;
			}
		}


		// put the data into the output vector

	}


}