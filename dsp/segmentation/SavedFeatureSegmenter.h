/*
 *  SavedFeatureSegmenter.h
 *  soundbite
 *
 *  Created by Mark Levy on 23/03/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

#include <vector>

#include "segment.h"
#include "Segmenter.h"
#include "hmm.h"

using std::vector;

class SavedFeatureSegmenterParams		
{
public:
	SavedFeatureSegmenterParams() : hopSize(0.2), windowSize(0.6), 
	nHMMStates(40), nclusters(10), histogramLength(15), neighbourhoodLimit(20) { }
	double hopSize;		// in secs
	double windowSize;	// in secs
	int nHMMStates;
	int nclusters;
	int histogramLength;
	int neighbourhoodLimit;
};

class SavedFeatureSegmenter : public Segmenter
{
public:
	SavedFeatureSegmenter(SavedFeatureSegmenterParams params);
	virtual ~SavedFeatureSegmenter();
	virtual void initialise(int samplerate);
	virtual int getWindowsize() { return static_cast<int>(windowSize * samplerate); }
	virtual int getHopsize() { return static_cast<int>(hopSize * samplerate); }
	virtual void extractFeatures(double* samples, int nsamples) { }
	void setFeatures(const vector<vector<double> >& f);		// provide the features yourself
	virtual void segment();		// segment into default number of segment-types
	void segment(int m);		// segment into m segment-types
	int getNSegmentTypes() { return nclusters; }
protected:
	void makeSegmentation(int* q, int len);
	
	model_t* model;				// the HMM
	int* q;						// the decoded HMM state sequence
	vector<vector<double> > histograms;	
	
	double hopSize;		// in seconds
	double windowSize;	// in seconds
	
	// HMM parameters
	int nHMMStates;
	
	// clustering parameters
	int nclusters;
	int histogramLength;
	int neighbourhoodLimit;
};