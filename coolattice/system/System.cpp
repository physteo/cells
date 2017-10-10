#include "System.h"
#define DEBUG0
#define DEBUG1


System::System()
{
	cells.resize(0);
	parts.resize(0);
	box = nullptr;
}

System::System(size_t n)
{
	cells.resize(n);
	parts.resize(0);
	box = nullptr;
}

System::System(CellColony* cellsIn)
{
	cells.reserve(cellsIn->size());
	for (size_t i = 0; i < cellsIn->size(); i++)
	{
		cells.push_back(cellsIn->at(i));
	}
	box = nullptr;
}


System::System(CellColony* cellsIn, Box* boxIn) : System{ cellsIn }
{
	constructPartsVector();
	box = boxIn;
}


System::System(CellColony* cellsIn, PartSpecs* partSpecsIn, Box* boxIn) : System{ cellsIn, boxIn }
{
	partSpecs = partSpecsIn;
	clearSubBoxes();
	setSubBoxes();
	
	//resolveOverlaps();
	
	//clearSubBoxes();
	//setSubBoxes();


}



void System::constructPartsVector()
{
	parts.resize(0);
	for (size_t c = 0; c < cells.size(); c++)
	{
		for (size_t p = 0; p < cells.at(c).getNumOfParts(); p++) {
			parts.push_back(&cells.at(c).getPart(p));
		}
	}
}

size_t System::getTotalNumOfParts()
{
	size_t total = 0;
	for (size_t i = 0; i < cells.size(); i++)
	{
		total += cells.at(i).getNumOfParts();
	}
	return total;
}

void System::printCoord()
{
	for (size_t c = 0; c < cells.size(); c++)
	{
		std::cout << "Cell " << c << "-----------------------" << std::endl;
		for (size_t p = 0; p < cells.at(c).getNumOfParts(); p++)
		{
			double& x = cells.at(c).getPart(p).position.x;
			double& y = cells.at(c).getPart(p).position.y;

			std::cout << x << ", " << y << std::endl;
		}
	}
}

void System::printCoordDiff()
{
	for (size_t c = 0; c < cells.size(); c++)
	{
		std::cout << "Cell " << c << "-----------------------" << std::endl;
		double& x1 = cells.at(c).getPart(0).position.x;
		double& y1 = cells.at(c).getPart(0).position.y;

		double& x2 = cells.at(c).getPart(1).position.x;
		double& y2 = cells.at(c).getPart(1).position.y;

		std::cout << x1 - x2 << ", " << y1 - y2 << std::endl;
	}
}


void System::setSubBoxes()
{
	// put particles in the subBoxes
//#ifdef OMP
//#pragma omp parallel for
//#endif
	for (int n = 0; n < parts.size(); n++) {
		Part* particle = parts.at(n);
		box->putPartInSubBox(particle, n);
	}

}


void System::clearSubBoxes()
{
	box->clearSubBoxes();
}


#define DEBUG

void System::computeForces(double dt)
{

#ifdef OMP
#pragma omp parallel for
#endif
	for (int n = 0; n < parts.size(); n++) {
		Part* part1 = parts.at(n);
		part1->velocity = Vector{ 0.0, 0.0 };
	}

	//test//double sigAA = 1.1;
	//test//double sigBB = 0.88;
	//test//double motility = 1.0;
	//test//double epsilon = 9.64832e-5;
	//test//double sigAB = .5*(sigAA + sigBB);
	//test//double cut = pow(2., 1. / 6.);
	//test//double rMaxSquared = 4.0;
	//test//double kappa = 0.482416;
	//test//LJForce    AA    = LJForce  { sigAA, epsilon, (sigAA * sigAA * cut * cut) };
	//test//LJForce    AB    = LJForce  { sigAB, epsilon, (sigAB * sigAB * cut * cut) };
	//test//LJForce    BA    = LJForce  { sigAB, epsilon, (sigAB * sigAB * cut * cut) };
	//test//LJForce    BB    = LJForce  { sigBB, epsilon, (sigBB * sigBB * cut * cut) };
	//test//FeneForce fene12 = FeneForce{ rMaxSquared, kappa };
	//test//CilForce  cil1   = CilForce { motility };
	//test//FeneForce fene21 = FeneForce{ rMaxSquared, kappa };
	

	// loop over all parts
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int n = 0; n < parts.size(); n++)
	{
		// get the part
		Part* part1 = parts.at(n);
		
		// apply one body forces
		for (size_t f = 0; f < partSpecs->oneBodyForces.at(part1->type).size(); f++)
		{
			//x/OneBodyForce* force = partSpecs->oneBodyForces.at(part1->type).at(f).get();
			OneBodyForce* force = partSpecs->oneBodyForces.at(part1->type).at(f); // .get() <- not here anymore cause i swtiched to raw pointers in order to make python interface
		
			force->updateForce(part1);
		}

#ifdef LIST
		// get the cell of this part
		BoxCell* subBox1 = part1->myBoxCell;
		// loop through the neighbours of subBox1
		for (int J = 0; J < 9; J++)
		{
			BoxCell* subBox2 = subBox1->neighbour[J];

			// loop through particles in this subBox
			Part* part2 = subBox2->head.next;
			while (part2 != nullptr)
			{
#else
		for (size_t m = 0; m < parts.size(); m++)
		{
			Part* part2 = parts.at(m);
#endif

				if (part2 != part1) {
					for (size_t f = 0; f < partSpecs->numberOfTwoBodyForces(part1, part2); f++)
					{
						const TwoBodyForce* force = partSpecs->getTwoBodyForce(part1, part2, f);
						force->updateForce(part1, part2, box);
					}

					//test//size_t type1 = part1->type;
					//test//size_t type2 = part2->type;
					//test//size_t cell1 = part1->cell;
					//test//size_t cell2 = part2->cell;
					//test//
					//test//if (cell1 == cell2)
					//test//{
					//test//	fene12.updateForce(part1, part2, box);
					//test//	if (type1 == 0)
					//test//	{
					//test//		// cil and fene1
					//test//		cil1.updateForce(part1, part2, box);
					//test//	}
					//test//}
					//test//else
					//test//{
					//test//	// apply intermol LJ 
					//test//	if (type1 == 0 && type2 == 0)
					//test//	{
					//test//		AA.updateForce(part1,part2,box);
					//test//	}
					//test//	else if (type1 == 0 && type2 == 1)
					//test//	{
					//test//		AB.updateForce(part1, part2, box);
					//test//	}
					//test//	else if (type1 == 1 && type2 == 0)
					//test//	{
					//test//		BA.updateForce(part1, part2, box);
					//test//	}
					//test//	else
					//test//	{
					//test//		BB.updateForce(part1, part2, box);
					//test//	}
					//test//}
					


#ifdef LIST
				}
				part2 = part2->next;
#endif
			}

		}

	box->remap(part1->position);
//#ifdef LIST
//	box->putPartInSubBox(part1, n);
//#endif

	}

}
#define NEWDEBUG
void System::updatePositions(double dt, bool update)
{
#ifdef OMP
#pragma omp parallel for
#endif
	for (int n = 0; n < parts.size(); n++)
	{
#ifdef NEWDEBUG
		Vector oldPosition = parts.at(n)->position;
#endif
		parts.at(n)->position += parts.at(n)->velocity * (dt/(FRICTION * MASS));

#ifdef NEWDEBUG
		Vector difference = parts.at(n)->position + (oldPosition * (-1) );
		double diff = Vector::dotProduct(difference, difference);
		if ( diff > 1 )
		{
			parts.at(n)->myBoxCell->printSubCellList(0);
			printf("parts size: %d\n", parts.size());
			printf("parts size: %d\n", parts.size());
			printf("diff: %e\n", diff);
		}
#endif
	}

#ifdef LIST
	if (update)
	{
		setSubBoxes();
	}
#endif

}

bool System::cellsAreBroken() const
{
	for (size_t c = 0; c < cells.size(); c++)
	{
		const Cell* cell = &cells.at(c);
		if (this->partSpecs->cellIsBroken(cell, box))
			return true;
	}
	return false;
}


void System::resolveOverlaps()
{
	bool overlaps = true;
	while (overlaps)
	{
		overlaps = false;
		for (int n = 0; n < parts.size(); n++)
		{
			// get the part
			Part* part1 = parts.at(n);
#ifdef LIST
			// get the cell of this part
			BoxCell* subBox1 = part1->myBoxCell;
			// loop through the neighbours of subBox1
			for (int J = 0; J < 9; J++)
			{
				BoxCell* subBox2 = subBox1->neighbour[J];

				// loop through particles in this subBox
				Part* part2 = subBox2->head.next;
				while (part2 != nullptr)
				{
#else
			for (size_t m = 0; m < parts.size(); m++)
			{
				Part* part2 = parts.at(m);
#endif
				if (part2 != part1 && part1->cell != part2->cell) {
					// check for overlaps
					double sig1 = this->partSpecs->getDiameter(part1->type);
					double sig2 = this->partSpecs->getDiameter(part2->type);
					double sig = 0.5 * (sig1 + sig2);
					
					Vector distance21Vec = part1->position - part2->position;
					double distance21 = sqrt(Vector::dotProduct(distance21Vec, distance21Vec));

					if (distance21 < sig)
					{
						// overlapping: need to correct
						double d = distance21;
						double x = ( (sig + 0.001)/d - 1.0)/(sig1 + sig2);
						part1->position += (distance21Vec * (sig2 * x)) ;
						part2->position -= (distance21Vec * (sig1 * x));

						overlaps = true;
					}

#ifdef LIST
				}
				part2 = part2->next;
#endif
			}

				}

			}

		}
}
	
