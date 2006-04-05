/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file is based on Don Cross's public domain FFT implementation.
*/

#include "FFT.h"
#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FFT::FFT()
{

}

FFT::~FFT()
{

}

void FFT::process(unsigned int p_nSamples, bool p_bInverseTransform, double *p_lpRealIn, double *p_lpImagIn, double *p_lpRealOut, double *p_lpImagOut)
{

    if(!p_lpRealIn || !p_lpRealOut || !p_lpImagOut) return;


    unsigned int NumBits;
    unsigned int i, j, k, n;
    unsigned int BlockSize, BlockEnd;

    double angle_numerator = 2.0 * M_PI;
    double tr, ti;

    if( !isPowerOfTwo(p_nSamples) )
    {
	return;
    }

    if( p_bInverseTransform ) angle_numerator = -angle_numerator;

    NumBits = numberOfBitsNeeded ( p_nSamples );


    for( i=0; i < p_nSamples; i++ )
    {
	j = reverseBits ( i, NumBits );
	p_lpRealOut[j] = p_lpRealIn[i];
	p_lpImagOut[j] = (p_lpImagIn == 0) ? 0.0 : p_lpImagIn[i];
    }


    BlockEnd = 1;
    for( BlockSize = 2; BlockSize <= p_nSamples; BlockSize <<= 1 )
    {
	double delta_angle = angle_numerator / (double)BlockSize;
	double sm2 = -sin ( -2 * delta_angle );
	double sm1 = -sin ( -delta_angle );
	double cm2 = cos ( -2 * delta_angle );
	double cm1 = cos ( -delta_angle );
	double w = 2 * cm1;
	double ar[3], ai[3];

	for( i=0; i < p_nSamples; i += BlockSize )
	{

	    ar[2] = cm2;
	    ar[1] = cm1;

	    ai[2] = sm2;
	    ai[1] = sm1;

	    for ( j=i, n=0; n < BlockEnd; j++, n++ )
	    {

		ar[0] = w*ar[1] - ar[2];
		ar[2] = ar[1];
		ar[1] = ar[0];

		ai[0] = w*ai[1] - ai[2];
		ai[2] = ai[1];
		ai[1] = ai[0];

		k = j + BlockEnd;
		tr = ar[0]*p_lpRealOut[k] - ai[0]*p_lpImagOut[k];
		ti = ar[0]*p_lpImagOut[k] + ai[0]*p_lpRealOut[k];

		p_lpRealOut[k] = p_lpRealOut[j] - tr;
		p_lpImagOut[k] = p_lpImagOut[j] - ti;

		p_lpRealOut[j] += tr;
		p_lpImagOut[j] += ti;

	    }
	}

	BlockEnd = BlockSize;

    }


    if( p_bInverseTransform )
    {
	double denom = (double)p_nSamples;

	for ( i=0; i < p_nSamples; i++ )
	{
	    p_lpRealOut[i] /= denom;
	    p_lpImagOut[i] /= denom;
	}
    }
}

bool FFT::isPowerOfTwo(unsigned int p_nX)
{
    if( p_nX < 2 ) return false;

    if( p_nX & (p_nX-1) ) return false;

    return true;
}

unsigned int FFT::numberOfBitsNeeded(unsigned int p_nSamples)
{	
    int i;

    if( p_nSamples < 2 )
    {
	return 0;
    }

    for ( i=0; ; i++ )
    {
	if( p_nSamples & (1 << i) ) return i;
    }
}

unsigned int FFT::reverseBits(unsigned int p_nIndex, unsigned int p_nBits)
{
    unsigned int i, rev;

    for(i=rev=0; i < p_nBits; i++)
    {
	rev = (rev << 1) | (p_nIndex & 1);
	p_nIndex >>= 1;
    }

    return rev;
}
