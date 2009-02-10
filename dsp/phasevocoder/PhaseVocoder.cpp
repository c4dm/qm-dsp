/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#include "PhaseVocoder.h"
#include "dsp/transforms/FFT.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PhaseVocoder::PhaseVocoder()
{

}

PhaseVocoder::~PhaseVocoder()
{

}

void PhaseVocoder::FFTShift(unsigned int size, double *src)
{
    const int hs = size/2;
    for (int i = 0; i < hs; ++i) {
        double tmp = src[i];
        src[i] = src[i + hs];
        src[i + hs] = tmp;
    }
}

void PhaseVocoder::process(unsigned int size, double *src, double *mag, double *theta)
{

    // Primary Interface to Phase Vocoder
    realOut = new double[ size ];
    imagOut = new double[ size ];

    FFTShift( size, src);
	
    coreFFT( size, src, 0, realOut, imagOut);

    getMagnitude( size/2, mag, realOut, imagOut);
    getPhase( size/2, theta, realOut, imagOut);

    delete [] realOut;
    delete [] imagOut;
}


void PhaseVocoder::coreFFT( unsigned int NumSamples, double *RealIn, double* ImagIn, double *RealOut, double *ImagOut)
{
    // This function is taken from a standard freeware implementation defined in FFT.h
    // TODO: Use  FFTW
    FFT::process( NumSamples,0, RealIn, ImagIn, RealOut, ImagOut );
}

void PhaseVocoder::getMagnitude(unsigned int size, double *mag, double *real, double *imag)
{	
    unsigned int j;

    for( j = 0; j < size; j++)
    {
	mag[ j ] = sqrt( real[ j ] * real[ j ] + imag[ j ] * imag[ j ]);
    }
}

void PhaseVocoder::getPhase(unsigned int size, double *theta, double *real, double *imag)
{
    unsigned int k;

    // Phase Angle "matlab" style 
    //Watch out for quadrant mapping  !!!
    for( k = 0; k < size; k++)
    {
	theta[ k ] = atan2( -imag[ k ], real[ k ]);
    }	
}
