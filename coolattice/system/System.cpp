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
	//TODO: now part.cell is not accessible//part1.getCell() = 0;
	//TODO: now part.cell is not accessible//part1.type = partID1;
	//TODO: now part.cell is not accessible//part2.cell = intra ? 0 : 1;
	//TODO: now part.cell is not accessible//part2.type = partID2;

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






//System::System()
//{
//	cells.reserve(0);
//	parts.resize(0);
//	box = nullptr;
//	measureTwoBodyForce = nullptr;
//	cycleLength = 0;
//	maxPredictedCells = 0;
//	cellCounter = 0;
//}

System::System(size_t n)
{
	cells.reserve(n);
	maxPredictedCells = n;

	parts.reserve(2 * n); // there will be probably 2 * n parts 
	box = nullptr;
	measureTwoBodyForce = nullptr;

	maxPredictedCells = n;
	cellCounter = n+1;
}

System::System(CellColony* cellsIn, size_t maxPredictedCellsIn)
{
#ifdef OBJECTPOOL
	cells.reserve(maxPredictedCellsIn);
	if (maxPredictedCellsIn < cellsIn->size())
	{
		maxPredictedCellsIn = 2 * cellsIn->size();
	}
	maxPredictedCells = maxPredictedCellsIn;
#else
	cells.reserve(cellsIn->size());
#endif
	for (size_t i = 0; i < cellsIn->size(); i++)
	{
		cells.push_back(cellsIn->at(i));
	}
	box = nullptr;
	measureTwoBodyForce = nullptr;

	cellCounter = maxPredictedCells + 1;
}


System::System(CellColony* cellsIn, Box* boxIn, size_t maxPredictedCellsIn) : System{ cellsIn, maxPredictedCellsIn }
{
	constructPartsVector();
	box = boxIn;
	std::cout << this << std::endl;
	measureTwoBodyForce = nullptr;

	clearSubBoxes();  
	setSubBoxes();
	std::cout << this << std::endl;

	
}


void System::addPartSpecs(PartSpecs* partSpecsIn)
{
	this->partSpecs.push_back(partSpecsIn);
}

void System::setCycleLength(unsigned short cycleLengthIn)
{
	cycleLength = cycleLengthIn;
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
			//if (cells.at(c).getPart(p).type > 1)
			//	std::cout << "lol" << std::endl;
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
	size_t n = 0;
	for (size_t c = 0; c < cells.size(); c++)
	{
		for (size_t p = 0; p < cells.at(c).getNumOfParts(); p++)
		{
			box->putPartInSubBox(&cells.at(c).getPart(p), n);
		}
		n += cells.at(c).getNumOfParts();
	}

	//for (size_t n = 0; n < parts.size(); n++) {
	//	Part* particle = parts.at(n);
	//	box->putPartInSubBox(particle, n);
	//}

}


void System::clearSubBoxes()
{
	box->clearSubBoxes();
}


void System::setPartSpecs(PartSpecs* partSpecsIn)
{ 
	//this->partSpecs = partSpecsIn;
	partSpecs.at(0) = partSpecsIn;
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

size_t System::computeStage(size_t time, size_t n)
{
	unsigned short partStageTime = parts.at(n)->stage;
	return ((time + partStageTime) % cycleLength) / (cycleLength/partSpecs.size());
}

size_t System::computeStage(size_t time, Part* part)
{
	unsigned short partStageTime = part->stage;
	return ((time + partStageTime) % cycleLength) / (cycleLength / partSpecs.size());
}

void System::computeForces(size_t time, double dt)
{
	resetVelocities();

	// loop over all parts //TODO URGENT PUT AGAIN THE OMP
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int n = 0; n < parts.size(); n++)
	{
		// get the part
		Part* part1 = parts.at(n);
		size_t stage = computeStage(time, n);


		// apply one body forces
		computeOneBodyForces(part1, partSpecs.at(stage));

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

				if (part2->type > 1 || part1->type > 1)
					int a = 1;

				computeTwoBodyForces(part1, part2, partSpecs.at(stage));

#ifdef LIST
			}
			part2 = part2->next;
#endif


		}

			}

	box->remap(part1->position);

		}

}



void System::computeForcesVoronoi(size_t time, double dt)
{
	resetVelocities();

	// loop over the subboxes
		// for each subbox:
		// construct voronoi of the subbox + its neighbours
		// loop over the particles inside the subbox and compute forces with the neighbours according to the voronoi

//#ifdef OMP
//#pragma omp parallel for schedule(guided)
//#endif
	for (int sb = 0; sb < box->getBoxCells().size(); sb++)
	{
		BoxCell* subBox1 = &box->getBoxCells().at(sb);
		// construct voronoi cell of the subBox and its neighbouring subBoxes
		double baseWidthX = (subBox1->upperRightCorner.x - subBox1->downLeftCorner.x);
		double baseWidthY = (subBox1->upperRightCorner.y - subBox1->downLeftCorner.y);

		double min_x = subBox1->downLeftCorner.x - baseWidthX;
		double min_y = subBox1->downLeftCorner.y - baseWidthY;


		double width_x = 3.0 * baseWidthX;
		double width_y = 3.0 * baseWidthY;

		// loop over neighbours and collect particles inside a vector.
		// if the neighbours are on the other side, then correct the position of the particles
		std::vector<Part*> voroBoxParts; //TODO: need to copy really?
		voroBoxParts.reserve(30);
		for (int J = 0; J < 9; J++) // can be optimized knowing that certain J are at the left, or right, etc
		{
			Vector addition{ 0.0,0.0 };

			BoxCell* subBox2 = subBox1->neighbour[J];
			// x
			if (subBox2->downLeftCorner.x > subBox1->upperRightCorner.x + baseWidthX / 2.0)
			{
				addition.x += -box->getBoxCellLengthX();
			}

			if (subBox2->upperRightCorner.x < subBox1->downLeftCorner.x - baseWidthX / 2.0)
			{
				addition.x += box->getBoxCellLengthX();
			}
			// y
			Vector upperLeftCorner2{ subBox2->downLeftCorner.x, subBox2->upperRightCorner.y };
			Vector downRightCorner2{ subBox2->upperRightCorner.x, subBox2->downLeftCorner.y };
			Vector upperLeftCorner1{ subBox1->downLeftCorner.x,   subBox1->upperRightCorner.y };
			Vector downRightCorner1{ subBox1->upperRightCorner.x, subBox1->downLeftCorner.y };
			// downLeftCorner ->downRightCorner
			//upperRightCorner->upperLeftCorner

			if (downRightCorner2.y > upperLeftCorner1.y + baseWidthY / 2.0)
			{
				addition.y += -box->getBoxCellLengthY();
			}

			if (upperLeftCorner2.y < downRightCorner1.y - baseWidthY / 2.0)
			{
				addition.y += box->getBoxCellLengthY();
			}

			// loop through particles in this subBox
			Part* part2 = subBox2->head.next;
			while (part2 != nullptr)
			{
				part2->position += addition;
				voroBoxParts.push_back(part2);
				part2 = part2->next;
			}

		}

		// now the vector voroBoxParts is ready and we can construct the voronoi box
		voro::container con(min_x, min_x + width_x, min_y, min_y + width_y, -0.5, 0.5, 1, 1, 1,
			false, false, false, 8);

		for (int i = 0; i < voroBoxParts.size(); i++)
			con.put(i, voroBoxParts.at(i)->position.x, voroBoxParts.at(i)->position.y, 0.0);

		voro::c_loop_subset loop = voro::c_loop_subset(con);
		loop.setup_box(	subBox1->downLeftCorner.x, subBox1->upperRightCorner.x,
						subBox1->downLeftCorner.y, subBox1->upperRightCorner.y,
						-0.5, 0.5, false); //TODO: false?
		voro::voronoicell_neighbor c;
		if (loop.start()) do if (con.compute_cell(c, loop)) {
			int ID1;
			ID1 = loop.pid();   // retrieve ID of particle
			Part* part1 = voroBoxParts.at(ID1);
			size_t stage = computeStage(time, part1);

			computeOneBodyForces(part1, partSpecs.at(stage));

			std::vector<int> neighbours;
			c.neighbors(neighbours);
			for (size_t j = 0; j < neighbours.size(); j++)
			{
				int ID2 = neighbours.at(j);
				if (ID2 > 0)
				{
					Part* part2 = voroBoxParts.at(ID2);
					// now i have part1 and part2
					computeTwoBodyForces(part1, part2, partSpecs.at(stage));

				}
				//	std::cout << "(" << parts.at(ID).x << ", " << parts.at(ID).y << ") - ";
			}

			box->remap(part1->position);
		} while (loop.inc());

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
					if (part1->type == measureTwoBodyForce->partID1 && part2->type == measureTwoBodyForce->partID2 && ((part1->getCell() == part2->getCell()) == (measureTwoBodyForce->intra)))
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
	//if (friction < 0)
	//	friction = std::numeric_limits<double>::infinity();
	//this->currentPartSpecs->setFriction(i, friction);
	assert(false);
}

double System::getTypeFriction(size_t i)
{
	//return this->currentPartSpecs->getFriction(i);
	return this->partSpecs.at(0)->getFriction(i);
}



#define NEWDEBUG
int System::updatePositions(size_t time, double dt, bool update)
{
	int result = 1;
#ifdef OMP
#pragma omp parallel for
#endif
	for (int n = 0; n < parts.size(); n++)
	{
		size_t pt = parts.at(n)->type;
		size_t stage = computeStage(time, n);

		double friction = this->partSpecs.at(stage)->getFriction(pt);
		double mass = this->partSpecs.at(stage)->partTypes.getPartTypes().at(pt).mass;
		

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
			result = 0;
		}
#endif
	}

#ifdef LIST
	if (update)
	{
		setSubBoxes();
	}
#endif
	return result;
}


void System::eraseRegionCells(double minX, double maxX, double minY, double maxY)
{
	bool deadCells = false;
	for (size_t c = 0; c < cells.size(); c++)
	{
		const Cell* cell = &cells.at(c);
		for (size_t p = 0; p < cell->getNumOfParts(); p++)
		{
			if (cell->getPart(p).position.x < minX || cell->getPart(p).position.x > maxX ||
				cell->getPart(p).position.y < minY || cell->getPart(p).position.y > maxY)
			{
				// remove particle from the subboxes
				for (size_t r = 0; r < cell->getNumOfParts(); r++)
				{
					// remove particle from the subboxes
					cells.at(c).getPart(r).remove();
				}
				cells.erase(c);
				c--;
				deadCells = true;
				break;
			}
		}
	}

	if (deadCells)
	{
		//clearSubBoxes();
		constructPartsVector();
		setSubBoxes();
	}

}

void System::eraseDeadCells()
{
	bool deadCells = false;
	for (size_t c = 0; c < cells.size(); c++)
	{
		const Cell* cell = &cells.at(c);
		if (this->partSpecs.at(0)->cellIsDead(cell, box))
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
		if (this->partSpecs.at(0)->cellDuplicates(cell, &newCells, box, cells.size() ))
		{
			for (size_t i = 0; i < newCells.size(); i++)
			{
				cells.push_back(newCells.at(i));
				for (size_t p = 0; p < newCells.at(i).getNumOfParts(); p++)
				{
					cells.back().getPart(p).cell = cellCounter;//1000000 - cells.size() - 1;
				}
				cellCounter++;
			}
			
			duplicated = true;
		}
		
	}

	if (duplicated)
	{
		//for (size_t c = 0; c < cells.size(); c++)
		//{
		//	Cell* cell = &cells.at(c);
		//	for (size_t r = 0; r < cell->getNumOfParts(); r++)
		//	{
		//		// remove particle from the subboxes
		//		cells.at(c).getPart(r).remove();
		//	}
		//}
		this->constructPartsVector();
		this->setSubBoxes();			// TODO: should be possible to update only the box which contains the dead cells
	}
}

//void System::resizeCellColony(size_t maxCells)
//{
//	//this->cells.resize(maxCells);
//	assert(false);
//	
//	//this->constructPartsVector();
//	//this->setSubBoxes();
//	//std::cout << "resized" << std::endl;
//}

bool System::cellsAreBroken() const
{
	for (size_t c = 0; c < cells.size(); c++)
	{
		const Cell* cell = &cells.at(c);
		if (this->partSpecs.at(0)->cellIsBroken(cell, box))
			return true;
	}
	return false;
}

bool System::twinCells() const
{
	std::vector<size_t> cellIDs;
	cellIDs.reserve(cells.size());
	for (size_t c = 0; c < cells.size(); c++)
	{
		cellIDs.emplace_back(cells.at(c).getPart(0).cell);
	}
	std::sort(cellIDs.begin(), cellIDs.end());

	for (size_t c = 0; c < cellIDs.size()-1; c++)
	{
		if (cellIDs.at(c) == cellIDs.at(c + 1))
		{
			return true;
		}
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
				if (part2 != part1 && part1->getCell() != part2->getCell()) {
					// check for overlaps
					double sig1 = this->partSpecs.at(0)->getDiameter(part1->type);
					double sig2 = this->partSpecs.at(0)->getDiameter(part2->type);
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

