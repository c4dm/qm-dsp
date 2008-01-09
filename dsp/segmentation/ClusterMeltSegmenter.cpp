/*
 *  ClusterMeltSegmenter.cpp
 *  soundbite
 *
 *  Created by Mark Levy on 23/03/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

#include <cfloat>
#include <cmath>

#include "ClusterMeltSegmenter.h"
#include "lib_constQ.h"
#include "cluster_segmenter.h"
#include "segment.h"

ClusterMeltSegmenter::ClusterMeltSegmenter(ClusterMeltSegmenterParams params) : window(NULL),
constq(NULL),
featureType(params.featureType),
windowSize(params.windowSize),
hopSize(params.hopSize),
fmin(params.fmin),
fmax(params.fmax),
nbins(params.nbins),
ncomponents(params.ncomponents),	// NB currently not passed - no. of PCA components is set in cluser_segmenter.c
nHMMStates(params.nHMMStates),
nclusters(params.nclusters),
histogramLength(params.histogramLength),
neighbourhoodLimit(params.neighbourhoodLimit)
{
}

void ClusterMeltSegmenter::initialise(int fs)
{
	samplerate = fs;
	if (featureType != FEATURE_TYPE_UNKNOWN)
	{
		ncoeff = static_cast<int>(ceil(nbins * (log(fmax / static_cast<double>(fmin))) / log(2.0)));
		constq = init_constQ(fmin, fmax, nbins, samplerate, ncoeff);	
	}
}

ClusterMeltSegmenter::~ClusterMeltSegmenter() 
{
	delete [] window;
	if (constq)
		close_constQ(constq);
}

void ClusterMeltSegmenter::extractFeatures(double* samples, int nsamples)
{
	// create a new window if needed
	if (!window || nsamples != windowLength)
	{
		if (window)
			delete [] window;
		window = hamming_p(nsamples);
		windowLength = nsamples;
	}	
	
	// copy the samples before windowing in case we need them for something else
	double* frame = new double[nsamples];
	for (int i = 0; i < nsamples; i++)
		frame[i] = samples[i] * window[i];
	
	// extract const-Q
	do_constQ(constq, frame, nsamples);
	int ncq = constq->ncoeff;
	
	delete [] frame;
	
	if (ncq == ncoeff)		// else feature extraction failed
	{
		vector<double> cq(ncq);
		for (int i = 0; i < ncq; i++)
			cq[i] = constq->absconstQtransform[i];
		features.push_back(cq);
	}
}

void ClusterMeltSegmenter::segment(int m)
{
	nclusters = m;
	segment();
}

void ClusterMeltSegmenter::setFeatures(const vector<vector<double> >& f)
{
	features = f;
	featureType = FEATURE_TYPE_UNKNOWN;
}

void ClusterMeltSegmenter::segment()
{
	if (constq)
	{
		close_constQ(constq);		// finished extracting features
		constq = NULL;
	}
	
	// for now copy the features to a native array and use the existing C segmenter...
	double** arrFeatures = new double*[features.size()];	
	for (int i = 0; i < features.size(); i++)
	{
		if (featureType == FEATURE_TYPE_UNKNOWN)
			arrFeatures[i] = new double[features[0].size()];
		else
			arrFeatures[i] = new double[ncoeff+1];	// allow space for the normalised envelope
		for (int j = 0; j < ncoeff; j++)
			arrFeatures[i][j] = features[i][j];	
	}
	
	q = new int[features.size()];
	
	if (featureType == FEATURE_TYPE_UNKNOWN)
		cluster_segment(q, arrFeatures, features.size(), features[0].size(), nHMMStates, histogramLength, 
						nclusters, neighbourhoodLimit);
	else
		constq_segment(q, arrFeatures, features.size(), nbins, ncoeff, featureType, 
					nHMMStates, histogramLength, nclusters, neighbourhoodLimit);
	
	// convert the cluster assignment sequence to a segmentation
	makeSegmentation(q, features.size());		
	
	// de-allocate arrays
	delete [] q;
	for (int i = 0; i < features.size(); i++)
		delete [] arrFeatures[i];
	delete [] arrFeatures;
	
	// clear the features
	clear();
}

void ClusterMeltSegmenter::makeSegmentation(int* q, int len)
{
	segmentation.segments.clear();
	segmentation.nsegtypes = nclusters;
	segmentation.samplerate = samplerate;
	
	Segment segment;
	segment.start = 0;
	segment.type = q[0];
	
	for (int i = 1; i < len; i++)
	{
		if (q[i] != q[i-1])
		{
			segment.end = i * getHopsize();
			segmentation.segments.push_back(segment);
			segment.type = q[i];
			segment.start = segment.end;
		}
	}
	segment.end = len * getHopsize();
	segmentation.segments.push_back(segment);
}

/*
void ClusterMeltSegmenter::mpeg7ConstQ()
{
	// convert to dB scale
	for (int i = 0; i < features.size(); i++)
		for (int j = 0; j < ncoeff; j++)
			features[i][j] = 10.0 * log10(features[i][j] + DBL_EPSILON);
	
	// normalise features and add the norm at the end as an extra feature dimension
	double maxnorm = 0;		// track the max of the norms
	for (int i = 0; i < features.size(); i++)
	{
		double norm = 0;
		for (int j = 0; j < ncoeff; j++)
			norm += features[i][j] * features[i][j];
		norm = sqrt(norm);
		for (int j = 0; j < ncoeff; j++)
			features[i][j] /= norm;
		features[i].push_back(norm);
		if (norm > maxnorm)
			maxnorm = norm;
	}
	
	// normalise the norms
	for (int i = 0; i < features.size(); i++)
		features[i][ncoeff] /= maxnorm;
}
*/
