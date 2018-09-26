#pragma once

#include "../../box/Box.h"
#include "../../particles/PartSpecs.h"
#include "../SystemTrajectory.h"


#include <cmath>

#include <gsl/gsl_histogram.h>
#include <gsl/gsl_histogram2d.h>

class Histogram2D
{
	const double THRESHOLD = 1e-10;

	std::vector<gsl_histogram2d*> m_histograms;

public:
	Histogram2D(size_t numHistograms, int nBinsXin, int nBinsYin, double xmin, double xmax, double ymin, double ymax)
	{
		m_histograms.resize(numHistograms);
		for (size_t i = 0; i < m_histograms.size(); i++)
		{
			m_histograms.at(i) = gsl_histogram2d_alloc(nBinsXin, nBinsYin);
			gsl_histogram2d_set_ranges_uniform(m_histograms.at(i), xmin, xmax, ymin, ymax);
		}
	}

	~Histogram2D()
	{
		for (size_t i = 0; i < m_histograms.size(); i++)
		{
			gsl_histogram2d_free(m_histograms.at(i));
		}
	}


	int accumulate(size_t whichHistogram, double x, double y, double weight)
	{
		return gsl_histogram2d_accumulate(m_histograms.at(whichHistogram), x, y, weight);
	}

	int scale(size_t whichHistogram, double scale)
	{
		return gsl_histogram2d_scale(m_histograms.at(whichHistogram), scale);
	}

	int divide(size_t whichHistogram1, size_t whichHistogram2)
	{
		return gsl_histogram2d_div(m_histograms.at(whichHistogram1), m_histograms.at(whichHistogram2));
	}

	void divideIfNotZero(size_t whichHistogram1, size_t whichHistogram2)
	{
		gsl_histogram2d* h1 = m_histograms.at(whichHistogram1);
		gsl_histogram2d* h2 = m_histograms.at(whichHistogram2);

		for (size_t i = 0; i < (h1->nx) * (h2->ny); i++)
		{
			if (h2->bin[i] > THRESHOLD) {
				h1->bin[i] /= h2->bin[i];
			}
			else {
				h1->bin[i] = 0;
			}
		}

	}

	void saveToFile(const std::string& filepath) const
	{
		FILE* file = fopen(filepath.c_str(), "w");

		gsl_histogram2d* reference = m_histograms.at(0);

		const size_t nx = reference->nx;
		const size_t ny = reference->ny;


		for (size_t i = 0; i < nx; i++)
		{
			for (size_t j = 0; j < ny; j++)
			{
				fprintf(file, "%g", reference->xrange[i]);
				putc(' ', file);
				fprintf(file, "%g", reference->xrange[i + 1]);
				putc(' ', file);
				fprintf(file, "%g", reference->yrange[j]);
				putc(' ', file);
				fprintf(file, "%g", reference->yrange[j + 1]);
				putc(' ', file);

				for (size_t k = 0; k < m_histograms.size(); k++) {
					fprintf(file, "%g", m_histograms.at(k)->bin[i * ny + j]);
					putc(' ', file);
				}
				putc('\n', file);
			}
			putc('\n', file);
		}
		fclose(file);
	}

};


class Analyzer
{
public:
	Analyzer(Box* box, const PartSpecs* partSpecs, SystemTrajectory* traj)
		: m_box(box), m_partSpecs(partSpecs), m_traj(traj) {}

	void computeCorrelationFunctions(const std::string& outfile, int numBins, double rMin, double rMax, int start, int end, int stride);
	void computeCorrelationFunctionsSingleDiskCells(const std::string& outfile, int numBins, double rMin, double rMax, int start, int end, int stride);
private:
	const double PI = 3.141592653589793;

	Box* m_box;
	const PartSpecs* m_partSpecs;
	SystemTrajectory* m_traj;

};