/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef PHASEVOCODER_H
#define PHASEVOCODER_H


class PhaseVocoder  
{
public:
    PhaseVocoder();
    virtual ~PhaseVocoder();

    void process( unsigned int size, double* src, double* mag, double* theta);

protected:
    void getPhase(unsigned int size, double *theta, double *real, double *imag);
    void coreFFT( unsigned int NumSamples, double *RealIn, double* ImagIn, double *RealOut, double *ImagOut);
    void getMagnitude( unsigned int size, double* mag, double* real, double* imag);
    void FFTShift( unsigned int size, double* src);

    double* imagOut;
    double* realOut;

};

#endif
