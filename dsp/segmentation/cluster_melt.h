#ifndef _CLUSTER_MELT_H
#define _CLUSTER_MELT_H
/*
 *  cluster_melt.h
 *  cluster_melt
 *
 *  Created by Mark Levy on 21/02/2006.
 *  Copyright 2006 Centre for Digital Music, Queen Mary, University of London. All rights reserved.
 *
 */

#include <stdlib.h>
#include <math.h>

void cluster_melt(double *h,		/* normalised histograms, as a vector in row major order */
				  int m,			/* number of dimensions (i.e. histogram bins) */
				  int n,			/* number of histograms */
				  double *Bsched,	/* inverse temperature schedule */
				  int t,			/* length of schedule */
				  int k,			/* number of clusters */
				  int l,			/* neighbourhood limit (supply zero to use default value) */
				  int *c			/* sequence of cluster assignments */
);

#endif