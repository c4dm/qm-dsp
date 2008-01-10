/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005 Nicolas Chetry, copyright 2008 QMUL.
    All rights reserved.
*/

#ifndef MFCC_H
#define MFCC_H

#include "base/Window.h"

struct MFCCConfig {
    int FS;
    int fftsize;
    int nceps;
    bool want_c0;
};

class MFCC
{
public:
    MFCC(MFCCConfig config);
    virtual ~MFCC();

    int process(int length, double *inframe, double *outceps);

    int getfftlength() const { return fftSize; }

private:
    /* Filter bank parameters */
    double  lowestFrequency; 
    int     linearFilters; 
    double  linearSpacing;
    int     logFilters;
    double  logSpacing;
    
    /* FFT length */
    int     fftSize;
    
    int     totalFilters;
    
    /* Misc. */
    int     samplingRate;
    int     nceps;
    
    /* MFCC vector */
    double  *ceps;
    
    double  **mfccDCTMatrix;
    double  **mfccFilterWeights;
    
    /* The analysis window */
    Window<double> *window;
    
    /* For the FFT */
    double* imagIn;		// always zero
    double* realOut;
    double* imagOut;
    
    /* Set if user want C0 */
    int WANT_C0;
};


#endif

