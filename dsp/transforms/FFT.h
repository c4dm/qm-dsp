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
    static void process(unsigned int nSamples, bool bInverseTransform,
                        const double *lpRealIn, const double *lpImagIn,
                        double *lpRealOut, double *lpImagOut);
    FFT();
    virtual ~FFT();

protected:
    static unsigned int reverseBits(unsigned int nIndex, unsigned int nBits);
    static unsigned int numberOfBitsNeeded( unsigned int nSamples );
    static bool isPowerOfTwo( unsigned int nX );
};

#endif
