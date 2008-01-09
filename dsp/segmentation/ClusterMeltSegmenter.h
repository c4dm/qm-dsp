/*
 *  ClusterMeltSegmenter.h
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
#include "lib_constQ.h"

using std::vector;

class ClusterMeltSegmenterParams		// defaults are sensible for 11025Hz with 0.2 second hopsize
{
public:
	ClusterMeltSegmenterParams() : featureType(FEATURE_TYPE_CONSTQ), hopSize(0.2), windowSize(0.6), fmin(62), fmax(16000), 
	nbins(8), ncomponents(20), 	nHMMStates(40), nclusters(10), histogramLength(15), neighbourhoodLimit(20) { }
	feature_types featureType;
	double hopSize;		// in secs
	double windowSize;	// in secs
	int fmin;
	int fmax;
	int nbins;
	int ncomponents;
	int nHMMStates;
	int nclusters;
	int histogramLength;
	int neighbourhoodLimit;
};

class ClusterMeltSegmenter : public Segmenter
{
public:
	ClusterMeltSegmenter(ClusterMeltSegmenterParams params);
	virtual ~ClusterMeltSegmenter();
	virtual void initialise(int samplerate);
	virtual int getWindowsize() { return static_cast<int>(windowSize * samplerate); }
	virtual int getHopsize() { return static_cast<int>(hopSize * samplerate); }
	virtual void extractFeatures(double* samples, int nsamples);
	void setFeatures(const vector<vector<double> >& f);		// provide the features yourself
	virtual void segment();		// segment into default number of segment-types
	void segment(int m);		// segment into m segment-types
	int getNSegmentTypes() { return nclusters; }
protected:
	//void mpeg7ConstQ();
	void makeSegmentation(int* q, int len);
	
	double* window;
	int windowLength;			// in samples
	constQ_t* constq;	
	model_t* model;				// the HMM
	//vector<int> stateSequence;
	//vector<int> segmentTypeSequence;
	int* q;						// the decoded HMM state sequence
	vector<vector<double> > histograms;	
	
	feature_types featureType;	
	double hopSize;		// in seconds
	double windowSize;	// in seconds
	
	// constant-Q parameters
	int fmin;
	int fmax;
	int nbins;
	int ncoeff;
	
	// PCA parameters
	int ncomponents;
	
	// HMM parameters
	int nHMMStates;
	
	// clustering parameters
	int nclusters;
	int histogramLength;
	int neighbourhoodLimit;
};
