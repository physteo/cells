#include "Analyzer.h"


static int nint(double a)
{
	return ((a) >= 0.0 ? (int)((a)+0.5) : (int)((a)-0.5));
}


void Analyzer::computeCorrelationFunctions(const std::string& outfile, int numBins, double rMin, double rMax, int start, int end, int stride)
{
	const size_t STAGETIMECORR = 0;
	const size_t STAGECORR = 1;
	const size_t RDF_PROPER = 2;
	const size_t RDF = 3;
	const size_t AXISCORR = 4;

	// 0-> correlation stagetime
	// 1-> correlation stage
	// 2-> g(r) from particle's axis
	// 3-> g(r) 
	Histogram2D histograms{ 5, numBins, numBins, rMin, rMax, rMin, rMax };

	double frames = double(floor((end - start) / stride));
	double bin_size_x = (rMax - rMin) / numBins;
	double bin_size_y = (rMax - rMin) / numBins;
	double num_particles = double(m_traj->at(0).size());
	double rho = num_particles / (m_box->getLx() * m_box->getLy());
	double rdf_normalization = frames * bin_size_x * bin_size_y * num_particles * rho;


	double cycleDurationDouble = (double) m_partSpecs->getStageDuration(0);

	
	// loop over time
	for (size_t t = start; t < end; t+=stride)
	{
		CellColony* cells = &m_traj->at(t);

		// double loop over cells
		for (size_t i = 0; i < cells->size(); i++)
		{
			for (size_t j = i + 1; j < cells->size(); j++)
			{
				// first displace the particles to the correct distance, taking into account PBC
				Cell& ci = cells->at(i);
				Cell& cj = cells->at(j);

				Vector position_i = ci.getPart(0).position;
				Vector position_j = cj.getPart(0).position;

				Vector distance_vec;
				double r_ij_squared = m_box->computeDistanceSquaredPBC(position_i, position_j, distance_vec);
				// now position them correctly with respect to each other, accounting for PBC
				position_j = position_i - distance_vec;

				double px_i = position_i.x;
				double py_i = position_i.y;
				double px_j = position_j.x;
				double py_j = position_j.y;

				Vector axis;
				double length = sqrt(m_box->computeDistanceSquaredPBC(ci.getPart(0).position, ci.getPart(1).position, axis));
				axis *= 1.0 / length;
			
				// cell i and cell j: compute distance (front-front) and coeff_ij
				double old_deltax = px_j - px_i;
				double old_deltay = py_j - py_i;


				double deltax = axis.y * old_deltax - axis.x * old_deltay;
				double deltay = axis.x * old_deltax + axis.y * old_deltay;

			
				double cycleTimeDifference = ((double)ci.getPart(0).currentStageTime) - ((double)cj.getPart(0).currentStageTime) +
					cycleDurationDouble * (((double)ci.getPart(0).currentStage) - ((double)cj.getPart(0).currentStage));

				double coeff_ij = cos((PI / cycleDurationDouble) * (cycleTimeDifference));

				double stagestage = 2.0 * ((!(!(ci.getPart(0).currentStage) != !(cj.getPart(0).currentStage))) - 0.5);
				//std::cout << ci.getPart(0).currentStage << "," << cj.getPart(0).currentStage << " -> " << stagestage  << std::endl;

				Vector axis_j;
				double length_j = sqrt(m_box->computeDistanceSquaredPBC(cj.getPart(0).position, cj.getPart(1).position, axis_j));
				axis_j *= 1.0 / length_j;

				double axisCorrelation = Vector::dotProduct(axis_j, axis);

				histograms.accumulate(STAGETIMECORR, +deltax, +deltay, coeff_ij);
				histograms.accumulate(STAGETIMECORR, -deltax, -deltay, coeff_ij);

				histograms.accumulate(STAGECORR, +deltax, +deltay, stagestage);
				histograms.accumulate(STAGECORR, -deltax, -deltay, stagestage);
				
				histograms.accumulate(RDF_PROPER, +deltax, +deltay, 1.0);
				histograms.accumulate(RDF_PROPER, -deltax, -deltay, 1.0);

				histograms.accumulate(AXISCORR, +deltax, +deltay, axisCorrelation);
				histograms.accumulate(AXISCORR, -deltax, -deltay, axisCorrelation);

				histograms.accumulate(RDF, old_deltax, old_deltay, 2.0);
				histograms.accumulate(RDF, old_deltax, old_deltay, 2.0);

			}
		}
	}

	// divide the result by the counts
	histograms.divideIfNotZero(STAGETIMECORR, RDF_PROPER);
	histograms.divideIfNotZero(STAGECORR, RDF_PROPER);
	histograms.divideIfNotZero(AXISCORR, RDF_PROPER);

	histograms.scale(RDF_PROPER, 1.0 / rdf_normalization);
	histograms.scale(RDF, 1.0 / rdf_normalization);


	histograms.saveToFile(outfile);
}


void Analyzer::computeCorrelationFunctionsSingleDiskCells(const std::string& outfile, int numBins, double rMin, double rMax, int start, int end, int stride)
{
	const size_t RDF = 0;

	// 0-> g(r) 
	Histogram2D histograms{ 1, numBins, numBins, rMin, rMax, rMin, rMax };

	double frames = double(floor((end - start) / stride));
	double bin_size_x = (rMax - rMin) / numBins;
	double bin_size_y = (rMax - rMin) / numBins;
	double num_particles = double(m_traj->at(0).size());
	double rho = num_particles / (m_box->getLx() * m_box->getLy());
	double rdf_normalization = frames * bin_size_x * bin_size_y * num_particles * rho;


	// loop over time
	for (size_t t = start; t < end; t += stride)
	{
		CellColony* cells = &m_traj->at(t);

		// double loop over cells this
		for (size_t i = 0; i < cells->size(); i++)
		{
			for (size_t j = i + 1; j < cells->size(); j++)
			{
				// first displace the particles to the correct distance, taking into account PBC
				Cell& ci = cells->at(i);
				Cell& cj = cells->at(j);

				Vector position_i = ci.getPart(0).position;
				Vector position_j = cj.getPart(0).position;

				Vector distance_vec;
				double r_ij_squared = m_box->computeDistanceSquaredPBC(position_i, position_j, distance_vec);
				// now position them correctly with respect to each other, accounting for PBC
				position_j = position_i - distance_vec;

				double px_i = position_i.x;
				double py_i = position_i.y;
				double px_j = position_j.x;
				double py_j = position_j.y;

				// cell i and cell j: compute distance (front-front) and coeff_ij
				double old_deltax = px_j - px_i;
				double old_deltay = py_j - py_i;

				histograms.accumulate(RDF, +old_deltax, +old_deltay, 1.0);
				histograms.accumulate(RDF, -old_deltax, -old_deltay, 1.0);

			}
		}
	}

	// divide the result by the counts
	histograms.scale(RDF, 1.0 / rdf_normalization);
	histograms.saveToFile(outfile);
}