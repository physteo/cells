#pragma once
#include <vector>
#include <iostream>

#include "../particles/CellColony.h"
#include "../utils/Hdf5.h"

class SystemTrajectory : public BinarySavable 
{
	std::vector<CellColony> m_trajectory;

public:
	SystemTrajectory() : m_trajectory{} {}

	void				push_back(const CellColony& cc);
	CellColony&			at(size_t i);
	const CellColony&   at(size_t i) const;
	CellColony			copyColony(size_t i) const { return this->at(i); };


	CellColony&			back();
	const CellColony	back() const;
	void				pop_back();

	void				resize(size_t i);
	void				reserve(size_t i);

	inline size_t       size() const { return m_trajectory.size(); };



	bool load(Hdf5* file, const char* fileName)       override;
	bool save(Hdf5* file, const char* fileName) const override;


};

