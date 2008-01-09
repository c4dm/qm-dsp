/*
 *  SavedFeatureSegmenter.cpp
 *  soundbite
 *
 *  Created by Mark Levy on 23/03/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

#include <cfloat>
#include <cmath>

#include "SavedFeatureSegmenter.h"
#include "cluster_segmenter.h"
#include "segment.h"

SavedFeatureSegmenter::SavedFeatureSegmenter(SavedFeatureSegmenterParams params) : windowSize(params.windowSize),
hopSize(params.hopSize),
nHMMStates(params.nHMMStates),
nclusters(params.nclusters),
histogramLength(params.histogramLength),
neighbourhoodLimit(params.neighbourhoodLimit)
{
}

void SavedFeatureSegmenter::initialise(int fs)
{
	samplerate = fs;
}

SavedFeatureSegmenter::~SavedFeatureSegmenter() 
{
}

void SavedFeatureSegmenter::segment(int m)
{
	nclusters = m;
	segment();
}

void SavedFeatureSegmenter::setFeatures(const vector<vector<double> >& f)
{
	features = f;
}

void SavedFeatureSegmenter::segment()
{
	// for now copy the features to a native array and use the existing C segmenter...
	double** arrFeatures = new double*[features.size()];	
	for (int i = 0; i < features.size(); i++)
	{
		arrFeatures[i] = new double[features[0].size()];	// allow space for the normalised envelope
		for (int j = 0; j < features[0].size(); j++)
			arrFeatures[i][j] = features[i][j];	
	}
	
	q = new int[features.size()];
	
	cluster_segment(q, arrFeatures, features.size(), features[0].size(), nHMMStates, histogramLength, 
						nclusters, neighbourhoodLimit);
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

void SavedFeatureSegmenter::makeSegmentation(int* q, int len)
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

