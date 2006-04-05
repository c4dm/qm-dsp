/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef CHROMAPROCESS_H
#define CHROMAPROCESS_H

#include <vector>

using namespace std;

class ChromaProcess  
{
public:
    void findHistMaxMin( vector<double> hist, double* max, int*maxIdx, double* min, int* minIdx );
    vector <int> mod( vector <int> input, int res );
    vector <int> getPeaks( vector <double> chroma, unsigned int BPO );
    int findChromaBias( vector<double> chromaVector, unsigned int BPO, unsigned int frames );
    ChromaProcess();
    virtual ~ChromaProcess();

};

#endif // !defined(CHROMAPROCESS_H)
