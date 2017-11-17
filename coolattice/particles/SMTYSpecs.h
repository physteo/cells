#pragma once
#include "../utils/Hdf5.h"
#include "PartSpecs.h"




// Schnyder-Molina-Tanaka-Yamamoto model for cells, with no noise(for the moment).
class SMTYSpecs : public PartSpecs
{
	// params
	//double m_sigAA;
	//double m_sigBB;
	double m_motility;
	double m_epsilon;
	double m_cut;
	double m_rMaxSquared;
	double m_kappa;
	//double m_frictionF;
	//double m_frictionB;
	//double m_massF;
	//double m_massB;

		
public:
	SMTYSpecs();
	SMTYSpecs(double sigAA, double sigBB, double motility,
		double epsilon, double cut, double rMaxSquared, double kappa, double frictionF, double frictionB, double massF, double massB);

	inline double getSigBB() { return this->partTypes.getPartTypes().at(1).sig; }

	bool load(Hdf5* file, const char* groupName)		  override;
	bool save(Hdf5* file, const char* groupName) const    override;
	bool cellIsBroken(const Cell* cell, const Box* box) const override;
	bool cellIsDead(const Cell* cell, const Box* box) override;
	bool cellDuplicates(Cell* cell, std::vector<Cell>* newCells, const Box* box, size_t& cellCounter, size_t cycleLength) const override;


private:
	void build();

};





//// One componenet Lennard-Jones system: should add noise otherwise the overdamped motion is "trivial"
//class MonoLJCreator : public PartSpecsCreator
//{
//	double m_sig;
//	double m_epsilon;
//	double m_cut;
//	double m_friction;
//public:
//	MonoLJCreator(double sig = 0.0, double epsilon = 0.0, double cut = 0.0, double friction = 0.0) :
//		m_sig(sig), m_epsilon(epsilon), m_cut(cut), m_friction(friction) {}
//
//	PartSpecs create();
//	virtual bool load(Hdf5* file, const char* groupName);
//	virtual bool save(Hdf5* file, const char* groupName) const;
//};





// TODO: binary WCA. Take from this code:
//		// binary Weeks-Chandler-Anderson
//		PartSpecs partSpecs(2);
//		double cut = pow(2., 1. / 6.);
//		double sigAA = 1.0;
//		double sigBB = 5. / 6.;
//		double sigAB = .5*(sigAA + sigBB);
//
//		partSpecs.forcesIntra.at(0) = std::make_unique<LJForce>( std::move( LJForce{ sigAA, 1.0, ( sigAA * sigAA * cut * cut) }));
//		partSpecs.forcesIntra.at(1) = std::make_unique<LJForce>( std::move( LJForce{ sigAB, 1.0, ( sigAB * sigAB * cut * cut) }));
//		partSpecs.forcesIntra.at(2) = std::make_unique<LJForce>( std::move( LJForce{ sigAB, 1.0, ( sigAB * sigAB * cut * cut) }));
//		partSpecs.forcesIntra.at(3) = std::make_unique<LJForce>( std::move( LJForce{ sigBB, 1.0, ( sigBB * sigBB * cut * cut) }));
//
//
//		partSpecs.partTypes.at(0).name = "A";
//		partSpecs.partTypes.at(1).name = "B";
//
//		partSpecs.partTypes.at(0).mass = 2.0;
//		partSpecs.partTypes.at(1).mass = 1.0;
//
//		partSpecs.partTypes.at(0).sig = sigAA;
//		partSpecs.partTypes.at(1).sig = sigBB;
//
//		//partSpecs.rCutGlobal = sigAA * cut;
//
//		return partSpecs;
