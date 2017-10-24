#include "GaussianRandomForce.h"

GaussianRandomForce::GaussianRandomForce(double varianceIn, double deltatIn,
	double frictionIn, double massIn, double KTIn) :
	variance(varianceIn), deltat(deltatIn), friction(frictionIn), mass(massIn), KT(KTIn)
{
	prefactor = sqrt(2.0 * KT * mass * friction * deltat) / deltat;     // because we update with:     dx = dt * Force / (friction * mass)
}																		// but with random force:      dx = sqrt(2 KT deltat    / (friction mass)) 



void GaussianRandomForce::updateForce(Part* part, const Box* box, Vector& forceVector) const
{
	double dx = gsl_ran_gaussian_ziggurat(g_rng, variance);
	double dy = gsl_ran_gaussian_ziggurat(g_rng, variance);

	forceVector = Vector{ dx, dy } * prefactor;
	part->velocity += forceVector;
}
