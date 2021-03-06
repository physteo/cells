#include "SystemTrajectory.h"



void SystemTrajectory::push_back(const CellColony& cc)
{
	m_trajectory.push_back(cc);
	//std::cout << this << std::endl;
	//
	//std::cout << &cc << std::endl;
	//std::cout << &this->at(0).at(0) << std::endl;
	//
	//std::cout << this->at(0).at(0).getPart(0).getCell() << std::endl;
}

CellColony& SystemTrajectory::at(size_t i)
{
	return m_trajectory.at(i);
}

const CellColony& SystemTrajectory::at(size_t i) const
{
	return m_trajectory.at(i);
}

CellColony& SystemTrajectory::back()
{
	return m_trajectory.back();
}

const CellColony SystemTrajectory::back() const
{
	return m_trajectory.back();
}

void SystemTrajectory::pop_back()
{
	m_trajectory.pop_back();
}


void SystemTrajectory::resize(size_t i)
{
	m_trajectory.resize(i);
}

void SystemTrajectory::reserve(size_t i)
{
	m_trajectory.reserve(i);
}

size_t SystemTrajectory::maxCells() const
{
	size_t maxCellsSize = 0;
	for (size_t i = 0; i < m_trajectory.size(); i++)
	{
		size_t snapshotSize = m_trajectory.at(i).totalNumberOfParts();
		if ( snapshotSize > maxCellsSize)
		{
			maxCellsSize = snapshotSize;
		}
	}
	return maxCellsSize;
}


bool SystemTrajectory::load(Hdf5* file, const char* name)
{	
	H5::Group group = file->openGroup(name);
	resize(group.getNumObjs());
	
	for (size_t t = 0; t < size(); t++)
	{
		// create new group
		char groupName[1024];
		sprintf(groupName, "%s/%u", name, (unsigned int)t);
		at(t).load(file, groupName);
	}
	return true;
}

bool SystemTrajectory::save(Hdf5* file, const char* name) const
{
	//if (!file->exists(name))
	//	return false;

	H5::Group group = file->createGroupNoThrow(name);

	for (size_t t = 0; t < size(); t++)
	{
		// create new group
		char groupName[1024];
		sprintf(groupName, "%s/%u", name, (unsigned int) t);
		//file->createGroup(groupName);		
		at(t).save(file, groupName);
	}

	return true;
}


