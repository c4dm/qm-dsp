/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008 QMUL
    All rights reserved.
*/

#include "KLDivergence.h"

double KLDivergence::distance(const vector<double> &m1,
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
