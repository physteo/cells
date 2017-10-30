#pragma once

#include<vector>

// this class is just an array of doubles, that gets exposed to the python interface so that we
// can use it to store input parameters
// (we need it because the maximum number of parameters allowed by the interface is 15)
struct Parameters
{
	std::vector<double> parameters;

public:
	Parameters() : parameters(0) {}

	void addParam(double param)
	{
		parameters.push_back(param);
	}

	double getParam(size_t i) const
	{
		return parameters.at(i);
	}

	size_t size() const
	{
		return parameters.size();
	}

};