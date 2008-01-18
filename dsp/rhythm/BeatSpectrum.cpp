/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008 Kurt Jacobson and QMUL.
    All rights reserved.
*/

#include "BeatSpectrum.h"

#include "maths/CosineDistance.h"

using std::vector;

vector<double> BeatSpectrum::process(const vector<vector<double> > &m)
{
    int origin = 0;
    int sz = m.size()/2;

    int i, j, k;

    vector<double> v(sz);
    for (i = 0; i < sz; ++i) v[i] = 0.0;

    CosineDistance cd;

    for (i = origin; i < origin + sz; ++i) {

        k = 0;

        for (j = i + 1; j < i + sz + 1; ++j) {

            v[k++] += cd.distance(m[i], m[j]);
        }
    }

    // normalize

    double max = 0.0;

    for (i = 0; i < sz; ++i) {
        if (v[i] > max) max = v[i];
    }

    if (max > 0.0) {
        for (i = 0; i < sz; ++i) {
            v[i] /= max;
        }
    }

    return v;
}

