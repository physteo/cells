#pragma once

#include <cmath>
#include <gsl/gsl_rng.h> 
#include <gsl/gsl_randist.h>

extern gsl_rng *g_rng;

#include "Force.h"


class GaussianRandomForce : public OneBodyForce
{
	double variance;
	double deltat;
	double friction;
	double mass;
	double KT;

public:
	GaussianRandomForce(double varianceIn = 1.0, double deltatIn = 1.0,
		double frictionIn = 1.0, double massIn = 1.0, double KTIn = 1.0);
	
	~GaussianRandomForce() {}

private:
	double prefactor;

public:
	void updateForce(Part* part, const Box* box, Vector& forceVector) const  override;

};