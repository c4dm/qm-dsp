/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file is based on Don Cross's public domain FFT implementation.
*/

#include "FFT.h"

#include "maths/MathUtilities.h"

#include <cmath>

#include <iostream>

FFT::FFT(int n) :
    m_n(n)
{
    if( !MathUtilities::isPowerOfTwo(m_n) )
    {
        std::cerr << "ERROR: FFT: Non-power-of-two FFT size "
                  << m_n << " not supported in this implementation"
                  << std::endl;
	return;
    }
}

FFT::~FFT()
{

}

FFTReal::FFTReal(int n) :
    m_n(n),
    m_fft(new FFT(n)),
    m_r(new double[n]),
    m_i(new double[n]),
    m_discard(new double[n])
{
    for (int i = 0; i < n; ++i) {
        m_r[i] = 0;
        m_i[i] = 0;
        m_discard[i] = 0;
    }
}

FFTReal::~FFTReal()
{
    delete m_fft;
    delete[] m_discard;
    delete[] m_r;
    delete[] m_i;
}

void
FFTReal::forward(const double *realIn,
                 double *realOut, double *imagOut)
{
    m_fft->process(false, realIn, 0, realOut, imagOut);
}

void
FFTReal::inverse(const double *realIn, const double *imagIn,
                 double *realOut)
{
    for (int i = 0; i < m_n/2 + 1; ++i) {
        m_r[i] = realIn[i];
        m_i[i] = imagIn[i];
        if (i > 0 && i < m_n/2) {
            m_r[m_n - i] = realIn[i];
            m_i[m_n - i] = -imagIn[i];
        }
    }
    m_fft->process(true, m_r, m_i, realOut, m_discard);
}

static int numberOfBitsNeeded(int p_nSamples)
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

static int reverseBits(int p_nIndex, int p_nBits)
{
    int i, rev;

    for(i=rev=0; i < p_nBits; i++)
    {
	rev = (rev << 1) | (p_nIndex & 1);
	p_nIndex >>= 1;
    }

    return rev;
}

void
FFT::process(bool p_bInverseTransform,
             const double *p_lpRealIn, const double *p_lpImagIn,
             double *p_lpRealOut, double *p_lpImagOut)
{
    if (!p_lpRealIn || !p_lpRealOut || !p_lpImagOut) return;

//    std::cerr << "FFT::process(" << m_n << "," << p_bInverseTransform << ")" << std::endl;

    int NumBits;
    int i, j, k, n;
    int BlockSize, BlockEnd;

    double angle_numerator = 2.0 * M_PI;
    double tr, ti;

    if( !MathUtilities::isPowerOfTwo(m_n) )
    {
        std::cerr << "ERROR: FFT::process: Non-power-of-two FFT size "
                  << m_n << " not supported in this implementation"
                  << std::endl;
	return;
    }

    if( p_bInverseTransform ) angle_numerator = -angle_numerator;

    NumBits = numberOfBitsNeeded ( m_n );



    for( i=0; i < m_n; i++ )
    {
	j = reverseBits ( i, NumBits );
	p_lpRealOut[j] = p_lpRealIn[i];
	p_lpImagOut[j] = (p_lpImagIn == 0) ? 0.0 : p_lpImagIn[i];
    }


    BlockEnd = 1;
    for( BlockSize = 2; BlockSize <= m_n; BlockSize <<= 1 )
    {
	double delta_angle = angle_numerator / (double)BlockSize;
	double sm2 = -sin ( -2 * delta_angle );
	double sm1 = -sin ( -delta_angle );
	double cm2 = cos ( -2 * delta_angle );
	double cm1 = cos ( -delta_angle );
	double w = 2 * cm1;
	double ar[3], ai[3];

	for( i=0; i < m_n; i += BlockSize )
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
	double denom = (double)m_n;

	for ( i=0; i < m_n; i++ )
	{
	    p_lpRealOut[i] /= denom;
	    p_lpImagOut[i] /= denom;
	}
    }
}

