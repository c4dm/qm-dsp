/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2008 Kurt Jacobson and QMUL.
    All rights reserved.
*/

#ifndef BEATSPECTRUM_H
#define BEATSPECTRUM_H

#include <vector>

/**
 * Given a matrix of "feature values", calculate a self-similarity
 * vector.  The resulting vector will have half as many elements as
 * the number of columns in the matrix.  This is based on the
 * SoundBite rhythmic similarity code.
 */

class BeatSpectrum
{
public:
    BeatSpectrum() { }
    ~BeatSpectrum() { }

    std::vector<double> process(const std::vector<std::vector<double> > &inmatrix);

};

#endif


