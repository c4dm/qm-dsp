#ifndef _CLUSTER_SEGMENTER_H
#define _CLUSTER_SEGMENTER_H

/*
 *  cluster_segmenter.h
 *  soundbite
 *
 *  Created by Mark Levy on 06/04/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "segment.h"
#include "cluster_melt.h"
#include "hmm.h"
#include "pca.h"

/* applies MPEG-7 normalisation to constant-Q features, storing normalised envelope (norm) in last feature dimension */
void mpeg7_constq(double** features, int nframes, int ncoeff);

/* converts constant-Q features to normalised chroma */
void cq2chroma(double** cq, int nframes, int ncoeff, int bins, double** chroma);

void create_histograms(int* x, int nx, int m, int hlen, double* h);

void cluster_segment(int* q, double** features, int frames_read, int feature_length, int nHMM_states, 
					 int histogram_length, int nclusters, int neighbour_limit);

void constq_segment(int* q, double** features, int frames_read, int bins, int ncoeff, int feature_type, 
			 int nHMM_states, int histogram_length, int nclusters, int neighbour_limit);


#endif