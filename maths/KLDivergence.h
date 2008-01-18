/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008 QMUL
    All rights reserved.
*/

#ifndef KLDIVERGENCE_H
#define KLDIVERGENCE_H

#include <vector>

using std::vector;

/**
 * Calculate a symmetrised Kullback-Leibler divergence of Gaussian
 * models based on mean and variance vectors.  All input vectors must
 * be of equal size.
 */

class KLDivergence
{
public:
    KLDivergence() { }
    ~KLDivergence() { }

    double distance(const vector<double> &means1,
		    const vector<double> &variances1,
		    const vector<double> &means2,
		    const vector<double> &variances2);
};

#endif

