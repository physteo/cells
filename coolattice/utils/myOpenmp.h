#pragma once
#include <iostream>
#include "../definitions.h"

class myOpenmp
{
	int m_numThreads;
public:
	myOpenmp(int numThreads) : m_numThreads(numThreads) {}
	
	void activate() const
	{
#ifdef OMP
		omp_set_dynamic(0);
		//size_t numThreads = inputJson["SIMULATION"]["threads"];
		std::cout << "Num processors " << omp_get_num_procs() << std::endl;
		std::cout << "Num of max threads " << omp_get_max_threads() << std::endl;
		omp_set_num_threads(m_numThreads);
#pragma omp parallel
		{
			printf("Num of threads begin used: %d\n", omp_get_num_threads());
		}
#else
		std::cout << "OpenMP is disabled. Please enable it from definitions.h by adding \"#define OMP\"." << std::endl;
#endif
	}
};
