#include "System.h"
#define DEBUG0
#define DEBUG1


MeasureTwoBodyForce::MeasureTwoBodyForce(PartSpecs* partSpecsIn, size_t partID1in, size_t partID2in,
	bool intraIn, size_t forceIDin)
{

	partSpecsMeasure = partSpecsIn;
	partID1 = partID1in;
	partID2 = partID2in;
	intra = intraIn;
	forceID = forceIDin;

	Part part1;
	Part part2;
	part1.cell = 0;
	part1.type = partID1;
	part2.cell = intra ? 0 : 1;
	part2.type = partID2;

	twoBodyForce = partSpecsIn->getTwoBodyForce(&part1, &part2, forceID);
	//twoBodyForce = partSpecsIn->oneBodyForces.at(0).at(0);
}

bool MeasureTwoBodyForce::load(Hdf5* file, const char* name)
{
	std::cerr << "Not possible to load force measurements in this version of the code." << std::endl;
	assert(false);
	return false;
}

bool MeasureTwoBodyForce::save(Hdf5* file, const char* name) const
{
	file->createNewGroup(name);
	H5::Group groupCells = file->openGroup(name);
	const H5::CompType& h5type = Hdf5types::getSimonType();

	for (size_t i = 0; i < data.size(); i++)
	{
		char stringName[1024];
		sprintf(stringName, "%s_%u", "t", (unsigned int)i);

		size_t LENGTH = this->data.at(i).size();
		int RANK = 1;
		hsize_t dim[] = { LENGTH };   /* Dataspace dimensions */
		H5::DataSpace space(RANK, dim);
		H5::DataSet* dataset = new H5::DataSet(groupCells.createDataSet(stringName, h5type, space));
		dataset->write(&data.at(i).at(0), h5type);
		delete dataset;
	}
	return true;
}






System::System()
{
	cells.resize(0);
	parts.resize(0);
	box = nullptr;
	measureTwoBodyForce = nullptr;
}

System::System(size_t n)
{
	cells.resize(n);
	parts.resize(0);
	box = nullptr;
	measureTwoBodyForce = nullptr;
}

System::System(CellColony* cellsIn)
{
#ifdef OBJECTPOOL
	cells.reserve(cellsIn->size());
#else
	cells.reserve(cellsIn->size());
#endif
	for (size_t i = 0; i < cellsIn->size(); i++)
	{
		cells.push_back(cellsIn->at(i));
	}
	box = nullptr;
	measureTwoBodyForce = nullptr;
}


System::System(CellColony* cellsIn, Box* boxIn) : System{ cellsIn }
{
	constructPartsVector();
	box = boxIn;
	measureTwoBodyForce = nullptr;
}


System::System(CellColony* cellsIn, PartSpecs* partSpecsIn, Box* boxIn) : System{ cellsIn, boxIn }
{
	partSpecs = partSpecsIn;
	clearSubBoxes();
	setSubBoxes();

	//resolveOverlaps();

	//clearSubBoxes();
	//setSubBoxes();

	measureTwoBodyForce = nullptr;
}


void System::registerTwoBodyForceMeasurement(MeasureTwoBodyForce* m)
{
	this->measureTwoBodyForce = m;
}


void System::resizePartsDataSlots(size_t slots)
{
	for (size_t n = 0; n < parts.size(); n++)
	{
		parts.at(n)->data.resize(slots);
	}
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
	for (size_t n = 0; n < parts.size(); n++) {
		Part* particle = parts.at(n);
		box->putPartInSubBox(particle, n);
	}

}


void System::clearSubBoxes()
{
	box->clearSubBoxes();
}


void System::setPartSpecs(PartSpecs* partSpecsIn)
{ 
	//std::cout << " changing to: " << partSpecsIn << std::endl;
	this->partSpecs = partSpecsIn;
}



#define DEBUG


void System::resetVelocities()
{
#ifdef OMP
#pragma omp parallel for
#endif
	for (int n = 0; n < parts.size(); n++) {
		Part* part1 = parts.at(n);
		part1->velocity = Vector{ 0.0, 0.0 };
	}

}



void System::computeOneBodyForces(Part* part1, const PartSpecs* specs)
{
	for (size_t f = 0; f < specs->oneBodyForces.at(part1->type).size(); f++)
	{
		//x/OneBodyForce* force = partSpecs->oneBodyForces.at(part1->type).at(f).get();
		OneBodyForce* force = specs->oneBodyForces.at(part1->type).at(f); // .get() <- not here anymore cause i swtiched to raw pointers in order to make python interface

		Vector addedForce;
		force->updateForce(part1, box, addedForce);
		//measureOneBodyForce.measure(part1->type, f, addedForce);
	}
}


void System::computeTwoBodyForces(Part* part1, const Part* part2, const PartSpecs* specs)
{
	for (size_t f = 0; f < specs->numberOfTwoBodyForces(part1, part2); f++)
	{
		const TwoBodyForce* force = specs->getTwoBodyForce(part1, part2, f);
		Vector addedForce;
		force->updateForce(part1, part2, box, addedForce);
		//measureTwoBodyForce.measure(part1->type, f, addedForce);
	}
}


void System::computeForces(double dt)
{
	resetVelocities();

	// loop over all parts
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int n = 0; n < parts.size(); n++)
	{
		// get the part
		Part* part1 = parts.at(n);

		// apply one body forces
		computeOneBodyForces(part1, partSpecs);

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

				computeTwoBodyForces(part1, part2, partSpecs);			

#ifdef LIST
			}
			part2 = part2->next;
#endif


		}

			}

	box->remap(part1->position);

		}

}



void System::collect()
	{
		if (measureTwoBodyForce == nullptr)
		{
			std::cerr << "No force to be measured has been registered." << std::endl;
		}


		//measureOneBodyForce->data.push_back(std::vector<Vector>{});
		measureTwoBodyForce->data.push_back(std::vector<Vector>{});
		//measureOneBodyForce->data.back().resize(this->parts.size());
		measureTwoBodyForce->data.back().resize(this->parts.size());


		// store the velocities. They will be restored into the particles velocity vector
		std::vector<Vector> velocities;
		velocities.resize(parts.size());
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
		for (int i = 0; i < parts.size(); i++)
		{
			velocities.at(i) = parts.at(i)->velocity;
			measureTwoBodyForce->data.back().at(i) = Vector{ 0.0, 0.0 };
		}

		// reset to zero so we can extract the forces
		resetVelocities();

		// measure the forces I want and store them into a vector
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
		for (int n = 0; n < parts.size(); n++)
		{
			Part* part1 = parts.at(n);
			// look at measureOneBodyForce
			//if (part1->type == measureOneBodyForce->partID)
			//{
			//	// apply force
			//	Vector addedVector{};
			//	measureOneBodyForce->oneBodyForce->updateForce(part1, addedVector);
			//}
			//measureOneBodyForce->data.back().push_back(part1->velocity);
			//part1->velocity = Vector{ 0.0,0.0 }; // necessary for the next measure (twoBodyForces) !

#ifdef LIST
			BoxCell* subBox1 = part1->myBoxCell;
			for (int J = 0; J < 9; J++)
			{
				BoxCell* subBox2 = subBox1->neighbour[J];
				Part* part2 = subBox2->head.next;
				while (part2 != nullptr)
				{
#else
			for (size_t m = 0; m < parts.size(); m++)
			{
				Part* part2 = parts.at(m);
#endif
				if (part2 != part1) {
					// measureTwoBodyForce
					if (part1->type == measureTwoBodyForce->partID1 && part2->type == measureTwoBodyForce->partID2 && ((part1->cell == part2->cell) == (measureTwoBodyForce->intra)))
					{
						Vector addedVector{};
						measureTwoBodyForce->twoBodyForce->updateForce(part1, part2, box, addedVector);
					}
#ifdef LIST
				}
				part2 = part2->next;
#endif
			}
				}
		measureTwoBodyForce->data.back().at(n) = part1->velocity;
		part1->velocity = Vector{ 0.0, 0.0 };
			}



	// restore velocities
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int i = 0; i < parts.size(); i++)
	{
		parts.at(i)->velocity = velocities.at(i);
	}

		}



#include<limits>
void System::setTypeFriction(size_t i, double friction)
{
	if (friction < 0)
		friction = std::numeric_limits<double>::infinity();
	this->partSpecs->setFriction(i, friction);
}

double System::getTypeFriction(size_t i)
{
	return this->partSpecs->getFriction(i);
}



#define NEWDEBUG
void System::updatePositions(double dt, bool update)
{
#ifdef OMP
#pragma omp parallel for
#endif
	for (int n = 0; n < parts.size(); n++)
	{
		size_t pt = parts.at(n)->type;
		double friction = this->partSpecs->getFriction(pt);
		double mass = this->partSpecs->partTypes.getPartTypes().at(pt).mass;

#ifdef NEWDEBUG
		Vector oldPosition = parts.at(n)->position;
#endif

		//parts.at(n)->position += parts.at(n)->velocity * (dt / (friction * mass));
		parts.at(n)->position += parts.at(n)->velocity * (dt / (friction));

#ifdef NEWDEBUG
		Vector difference = parts.at(n)->position + (oldPosition * (-1));
		double diff = Vector::dotProduct(difference, difference);
		if (diff > 1)
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


void System::eraseDeadCells()
{
	bool deadCells = false;
	for (size_t c = 0; c < cells.size(); c++)
	{
		const Cell* cell = &cells.at(c);
		if (this->partSpecs->cellIsDead(cell, box))
		{
			cells.erase(c);
			c--;
			deadCells = true;
		}
	}

	if (deadCells)
	{
		this->constructPartsVector();
		this->setSubBoxes();			// TODO: should be possible to update only the box which contains the dead cells
	}
}


void System::duplicateCells()
{
	bool duplicated = false;
	for (size_t c = 0; c < cells.size(); c++)
	{
		Cell* cell = &cells.at(c);

		std::vector<Cell> newCells;
		if (this->partSpecs->cellDuplicates(cell, &newCells, box, cells.size() ))
		{
			for (size_t i = 0; i < newCells.size(); i++)
			{
				cells.push_back(newCells.at(i));
				for (size_t p = 0; p < newCells.at(i).getNumOfParts(); p++)
				{
					cells.back().getPart(p).cell = cells.size() - 1;
				}
			}
			
			duplicated = true;
		}
		
	}

	if (duplicated)
	{
		this->constructPartsVector();
		this->setSubBoxes();			// TODO: should be possible to update only the box which contains the dead cells
	}
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
		for (size_t n = 0; n < parts.size(); n++)
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
						double x = ((sig + 0.001) / d - 1.0) / (sig1 + sig2);
						part1->position += (distance21Vec * (sig2 * x));
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

