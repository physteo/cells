#include "Analyzer.h"




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

				//Vector np_i = { cosine * px_i - sine * py_i, sine * px_i + cosine * py_i };
				//Vector np_j = { cosine * px_j - sine * py_j, sine * px_j + cosine * py_j };
			
				// cell i and cell j: compute distance (front-front) and coeff_ij
				double old_deltax = px_j - px_i;
				double old_deltay = py_j - py_i;


				double deltax = axis.y * old_deltax - axis.x * old_deltay;
				double deltay = axis.x * old_deltax + axis.y * old_deltay;

				
				if ((deltax > rMin) && (deltax < rMax) &&
					(deltay > rMin) && (deltay < rMax)
					)
				{
					double cycleTimeDifference = ((double)ci.getPart(0).currentStageTime) - ((double)cj.getPart(0).currentStageTime) +
						cycleDurationDouble * (((double)ci.getPart(0).currentStage) - ((double)cj.getPart(0).currentStage));

					double coeff_ij = cos((PI / cycleDurationDouble) * (cycleTimeDifference));

					double stagestage = 2.0 * ((!(!(ci.getPart(0).currentStage) != !(cj.getPart(0).currentStage))) - 0.5);
					//std::cout << ci.getPart(0).currentStage << "," << cj.getPart(0).currentStage << " -> " << stagestage  << std::endl;

					Vector axis_j;
					double length_j = sqrt(m_box->computeDistanceSquaredPBC(cj.getPart(0).position, cj.getPart(1).position, axis_j));
					axis_j *= 1.0 / length_j;

					double axisCorrelation = Vector::dotProduct(axis_j, axis);

					histograms.accumulate(STAGETIMECORR, deltax, deltay, coeff_ij);
					histograms.accumulate(STAGECORR, deltax, deltay, stagestage);
					histograms.accumulate(RDF_PROPER, deltax, deltay, 2.0);
					histograms.accumulate(AXISCORR, deltax, deltay, axisCorrelation);

				}

				if ((old_deltax > rMin) && (old_deltax < rMax) &&
					(old_deltay > rMin) && (old_deltay < rMax)
					)
				{
					histograms.accumulate(RDF, old_deltax, old_deltay, 2.0);
				}

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
	// create two files
	// copy the histograms into vectors and the write them together into a file


	//FILE* file1 = fopen(outfile.c_str(), "w");
	//std::string out2 = outfile;
	//std::string out3 = outfile;
	//std::string out4 = outfile;
	//out2 += "-counts";
	//out3 += "-stages";
	//out4 += "-abscounts";
	//FILE* file2 = fopen(  out2.c_str(), "w");
	//FILE* file3 = fopen(  out3.c_str(), "w");
	//FILE* file4 = fopen(  out4.c_str(), "w");
	//
	//if ( (file1 == NULL) || (file2 == NULL)  || (file3 == NULL) || (file4 == NULL))
	//{
	//	std::cerr << "Error opening file!" << std::endl;
	//	exit(1);
	//}
	//
	//gsl_histogram2d_fprintf(file1, histogram, "%g", "%g");
	//gsl_histogram2d_fprintf(file2, histogram_counts, "%g", "%g");
	//gsl_histogram2d_fprintf(file3, histogram_stages, "%g", "%g");
	//gsl_histogram2d_fprintf(file4, histogram_abs_counts, "%g", "%g");
	//
	//
	//fclose(file1);
	//fclose(file2);
	//fclose(file3);
	//fclose(file4);

	//gsl_histogram2d_free(histogram);
	//gsl_histogram2d_free(histogram_stages);
	//gsl_histogram2d_free(histogram_counts);
	//gsl_histogram2d_free(histogram_abs_counts);
}

