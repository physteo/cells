#include "System.h"
#define DEBUG0
#define DEBUG1


System::System(size_t n)
{
	cells.reserve(n);
	maxPredictedCells = n;

	parts.reserve(2 * n); // there will be probably 2 * n parts 
	box = nullptr;

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

	cellCounter = maxPredictedCells + 1;
}


System::System(CellColony* cellsIn, Box* boxIn, size_t maxPredictedCellsIn) : System{ cellsIn, maxPredictedCellsIn }
{
	constructPartsVector();
	box = boxIn;
	std::cout << this << std::endl;

	clearSubBoxes();  
	setSubBoxes();
	
}


void System::addPartSpecs(PartSpecs* partSpecsIn)
{
	this->partSpecs.push_back(partSpecsIn);
}


void System::setCycleLength(unsigned short cycleLengthIn)
{
	cycleLength = cycleLengthIn;
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
	
	box->putAllPartsInSubBox(&cells);

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


void System::computeOneBodyForces(Part* part1)
{
	partSpecs.at(0)->computeOneBodyForces(part1, box);
}


void System::computeTwoBodyForces(Part* part1, const Part* part2)
{
	partSpecs.at(0)->computeTwoBodyForces(part1, part2, box);
}


void System::updateStages(size_t time)
{
#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int c = 0; c < cells.size(); c++)
	{
		// update the stage of the particle
		partSpecs.at(0)->updateStage(time, &cells.at(c));
		//for(size_t n = 0; n < cells.at(c).getNumOfParts(); n++)
		//	cells.at(c).getPart(n).currentStageTime++;
	}
}
#ifdef DD
void System::diameterDynamics(size_t init, double dt)
{
	if (init == 0)
	{
		for (size_t i = 0; i < parts.size(); i++)
		{
			this->sigmaSpeeds.push_back(0.0);
		}
	}

	double omega = 0.1;
	double xi = 1.0;
	double sigBm = 0.5;
	double sigBM = 1.2;
	double sigFm = 0.8;
	double sigFM = 1.4;
	double lengthMax = 0.37;

#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int c = 0; c < this->cells.size(); c++)
	{
		Cell* cell = &this->cells.at(c);
		Vector rbf;
		double rbfModule = sqrt(this->box->computeDistanceSquaredPBC(cell->getPart(0).position, cell->getPart(1).position, rbf));
		
		double l0F = (rbfModule * (sigFM - sigFm) + lengthMax * sigFm) / lengthMax;
		double l0B = (rbfModule * (sigBM - sigBm) + lengthMax * sigBm) / lengthMax;

		double& sigmaF0 = cell->getPart(0).currentSigma;
		double& sigmaB0 = cell->getPart(1).currentSigma;

		double&  velF0   = this->sigmaSpeeds.at(c + 0);
		double&  velB0   = this->sigmaSpeeds.at(c + 1);

		double coeff_F = omega * (sigmaF0 - l0F) + velF0;
		double coeff_B = omega * (sigmaB0 - l0B) + velB0;

		double exp_dt_omega = exp(-dt * omega);//1 + (-dt * omega) + (-dt * omega)*(-dt * omega)/2.0 + (-dt * omega)*(-dt * omega)*(-dt * omega)/6.0;

		sigmaB0 = exp_dt_omega * (sigmaB0 - l0B + dt * coeff_B) + l0B;
		sigmaF0 = exp_dt_omega * (sigmaF0 - l0F + dt * coeff_F) + l0F;

		velF0 = -(sigmaF0 - l0F) * omega + exp_dt_omega * coeff_F;
		velB0 = -(sigmaB0 - l0B) * omega + exp_dt_omega * coeff_B;


		//double xF = sigmaF - l0F;
		//double xB = sigmaB - l0B;
		//
		//// update speeds
		//this->sigmaSpeeds.at(c + 0) = this->sigmaSpeeds.at(c + 0) - omega2 * dt * xF - 2 * omega2 * xi * this->sigmaSpeeds.at(c + 0) * dt;
		//this->sigmaSpeeds.at(c + 1) = this->sigmaSpeeds.at(c + 1) - omega2 * dt * xB - 2 * omega2 * xi * this->sigmaSpeeds.at(c + 1) * dt;
		//
		//// update positions
		//xF = this->sigmaSpeeds.at(c + 0) * dt;
		//xB = this->sigmaSpeeds.at(c + 1) * dt;
		//
		//sigmaF = xF + l0F;
		//sigmaB = xB + l0B;

	}

}
// dv = -2 si omega2 dx - omega2 x dt
// dv = f(x) * dt
// dx = v * dt
//
//
//
#endif


void System::computeForces(size_t time, double dt)
{
	resetVelocities();
//	updateStages(time);

#ifdef DD
		diameterDynamics(time, dt);
#endif

#ifdef OMP
#pragma omp parallel for schedule(guided)
#endif
	for (int n = 0; n < parts.size(); n++)
	{
		// get the part
		Part* part1 = parts.at(n);

		// apply one body forces
		computeOneBodyForces(part1);


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
					computeTwoBodyForces(part1, part2);
#ifdef LIST
				}
				part2 = part2->next;
#endif
			}

		}

	}

	


}



// voronoi stuff
/*
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
			size_t stage = part1->currentStage;

			//x//computeOneBodyForces(part1, partSpecs.at(stage));

			std::vector<int> neighbours;
			c.neighbors(neighbours);
			for (size_t j = 0; j < neighbours.size(); j++)
			{
				int ID2 = neighbours.at(j);
				if (ID2 > 0)
				{
					Part* part2 = voroBoxParts.at(ID2);
					// now i have part1 and part2
					//x//computeTwoBodyForces(part1, part2, partSpecs.at(stage));

				}
				//	std::cout << "(" << parts.at(ID).x << ", " << parts.at(ID).y << ") - ";
			}

			box->remap(part1->position);
		} while (loop.inc());

	}

}
*/



#include<limits>
void System::setTypeFriction(size_t i, double friction)
{
	//if (friction < 0)
	//	friction = std::numeric_limits<double>::infinity();
	//this->currentPartSpecs->setFriction(i, friction);
	assert(false);
}

double System::getTypeFriction(size_t stage, size_t i)
{
	//return this->currentPartSpecs->getFriction(i);
	return this->partSpecs.at(0)->getFriction(stage, i);
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
		//x//size_t stage = computeStage(time, n);
		//x//double friction = this->partSpecs.at(stage)->getFriction(pt);
		//x//double mass = this->partSpecs.at(stage)->partTypes.getPartTypes().at(pt).mass;

		double friction = this->partSpecs.at(0)->getFriction(parts.at(n));
		double mass		= this->partSpecs.at(0)->getMass(parts.at(n));

#ifdef NEWDEBUG
		Vector oldPosition = parts.at(n)->position;
		//Vector currentVelocity = parts.at(n)->velocity;
		//if (currentVelocity.x > 20 || currentVelocity.y > 20)
		//	std::cout << "problem" << std::endl;
#endif

		//parts.at(n)->position += parts.at(n)->velocity * (dt / (friction * mass));
		parts.at(n)->position += parts.at(n)->velocity * (dt / (friction));
		this->box->remap(parts.at(n)->position); // TODO: URGENT. VERIFY THIS MAKES SENSE!

#ifdef NEWDEBUG
		Vector difference;// = parts.at(n)->position + (oldPosition * (-1));
		box->computeDistanceVectorPBC(oldPosition, parts.at(n)->position, difference);
		double diff = Vector::dotProduct(difference, difference);
		if (diff > 1)
		{
			std::cout << "part: " << n << std::endl;
			std::cout << "criminal part position: " << parts.at(n)->position.x << "," << parts.at(n)->position.y << std::endl;
			parts.at(n)->myBoxCell->printSubCellList(0);
			printf("parts size: %d\n", parts.size());
			printf("diff: %e\n", diff);
			result = 1;
		}
#endif
	} 

#ifdef LIST
	if (update)
	{
		setSubBoxes();
	}
#endif


	// URGENT: remove
	// debug
	//Vector pos1 = cells.at(0).getPart(0).position;
	//Vector pos2 = cells.at(0).getPart(1).position;
	//Vector distance = pos1 - pos2;
	//std::cout << "Distance: " << sqrt(Vector::dotProduct(distance, distance)) << std::endl;
	// end of URGENT


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
		std::cout << "Some cells died" << std::endl;
		this->constructPartsVector();
		this->setSubBoxes();			// TODO: should be possible to update only the box which contains the dead cells
	}
}

// loops over all cells and check if they are at the end of a division stage, using the partSpecs method "endOfDivisionStage".
// if a cell is at the end of a division stage, then it divides
void System::divideCells()
{
	bool divided = false;
	for (size_t c = 0; c < cells.size(); c++)
	{
		Cell* cell = &cells.at(c);

		if (partSpecs.at(0)->endOfSuccessfullDivisionStage(cell, box))
		{
			// cell divides
			std::vector<Cell> newCells;
			
			// new cells are created and put into newCells
			this->partSpecs.at(0)->cellDivides(cell, &newCells, box, cellCounter, cycleLength);

			// the vector of new cells is inserted into the cells vector
			for (size_t i = 0; i < newCells.size(); i++)
			{
				cells.push_back(newCells.at(i));
			}

			divided = true;
		}

		
	}

	// if new cells were created, update the parts vector and the subBoxes
	if (divided)
	{
		this->constructPartsVector();
		this->setSubBoxes();			// TODO: should be possible to update only the box which contains the new cells
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
