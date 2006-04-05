/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file is based on Don Cross's public domain FFT implementation.
*/

#ifndef FFT_H
#define FFT_H

class FFT  
{
public:
    static void process(unsigned int p_nSamples, bool p_bInverseTransform,
                        double *p_lpRealIn, double *p_lpImagIn,
                        double *p_lpRealOut, double *p_lpImagOut);
    FFT();
    virtual ~FFT();

protected:
    static unsigned int reverseBits(unsigned int p_nIndex, unsigned int p_nBits);
    static unsigned int numberOfBitsNeeded( unsigned int p_nSamples );
    static bool isPowerOfTwo( unsigned int p_nX );
};

#endif
