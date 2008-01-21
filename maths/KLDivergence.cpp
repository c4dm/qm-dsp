/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008 QMUL
    All rights reserved.
*/

#include "KLDivergence.h"

#include <cmath>

double KLDivergence::distanceGaussian(const vector<double> &m1,
                                      const vector<double> &v1,
                                      const vector<double> &m2,
                                      const vector<double> &v2)
{
    int sz = m1.size();

    double d = -2.0 * sz;

    for (int k = 0; k < sz; ++k) {
        d += v1[k] / v2[k] + v2[k] / v1[k];
        d += (m1[k] - m2[k]) * (1.0 / v1[k] + 1.0 / v2[k]) * (m1[k] - m2[k]);
    }

    d /= 2.0;

    return d;
}

double KLDivergence::distanceDistribution(const vector<double> &d1,
                                          const vector<double> &d2,
                                          bool symmetrised)
{
    int sz = d1.size();

    double d = 0;
    double small = 1e-20;
    
    for (int i = 0; i < sz; ++i) {
        d += d1[i] * log10((d1[i] + small) / (d2[i] + small));
    }

    if (symmetrised) {
        d += distanceDistribution(d2, d1, false);
    }

    return d;
}

