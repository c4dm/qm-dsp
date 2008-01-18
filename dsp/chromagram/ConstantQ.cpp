/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */
/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#include "ConstantQ.h"
#include "dsp/transforms/FFT.h"

#include <iostream>

//---------------------------------------------------------------------------
// nextpow2 returns the smallest integer n such that 2^n >= x.
static double nextpow2(double x) {
    double y = ceil(log(x)/log(2.0));
    return(y);
}

static double squaredModule(const double & xx, const double & yy) {
    return xx*xx + yy*yy;
}

//----------------------------------------------------------------------------

ConstantQ::ConstantQ( CQConfig Config ) 
{
    initialise( Config );
}

ConstantQ::~ConstantQ()
{
    deInitialise();
}

//----------------------------------------------------------------------------
void ConstantQ::sparsekernel()
{
    //generates spectral kernel matrix (upside down?)
    // initialise temporal kernel with zeros, twice length to deal w. complex numbers

    double* hammingWindowRe = new double [ m_FFTLength ];
    double* hammingWindowIm = new double [ m_FFTLength ];
    double* transfHammingWindowRe = new double [ m_FFTLength ];
    double* transfHammingWindowIm = new double [ m_FFTLength ];

    for (unsigned u=0; u < m_FFTLength; u++) 
    {
	hammingWindowRe[u] = 0;
	hammingWindowIm[u] = 0;
    }


    // Here, fftleng*2 is a guess of the number of sparse cells in the matrix
    // The matrix K x fftlength but the non-zero cells are an antialiased
    // square root function. So mostly is a line, with some grey point.
    m_sparseKernelIs.reserve( m_FFTLength*2 );
    m_sparseKernelJs.reserve( m_FFTLength*2 );
    m_sparseKernelRealValues.reserve( m_FFTLength*2 );
    m_sparseKernelImagValues.reserve( m_FFTLength*2 );
	
    // for each bin value K, calculate temporal kernel, take its fft to
    //calculate the spectral kernel then threshold it to make it sparse and 
    //add it to the sparse kernels matrix
    double squareThreshold = m_CQThresh * m_CQThresh;

    FFT m_FFT;
	
    for (unsigned k = m_uK; k--; ) 
    {
        for (unsigned u=0; u < m_FFTLength; u++) 
        {
            hammingWindowRe[u] = 0;
            hammingWindowIm[u] = 0;
        }
        
	// Computing a hamming window
	const unsigned hammingLength = (int) ceil( m_dQ * m_FS / ( m_FMin * pow(2,((double)(k))/(double)m_BPO)));

        unsigned origin = m_FFTLength/2 - hammingLength/2;

	for (unsigned i=0; i<hammingLength; i++) 
	{
	    const double angle = 2*PI*m_dQ*i/hammingLength;
	    const double real = cos(angle);
	    const double imag = sin(angle);
	    const double absol = hamming(hammingLength, i)/hammingLength;
	    hammingWindowRe[ origin + i ] = absol*real;
	    hammingWindowIm[ origin + i ] = absol*imag;
	}

        for (unsigned i = 0; i < m_FFTLength/2; ++i) {
            double temp = hammingWindowRe[i];
            hammingWindowRe[i] = hammingWindowRe[i + m_FFTLength/2];
            hammingWindowRe[i + m_FFTLength/2] = temp;
            temp = hammingWindowIm[i];
            hammingWindowIm[i] = hammingWindowIm[i + m_FFTLength/2];
            hammingWindowIm[i + m_FFTLength/2] = temp;
        }
    
	//do fft of hammingWindow
	m_FFT.process( m_FFTLength, 0, hammingWindowRe, hammingWindowIm, transfHammingWindowRe, transfHammingWindowIm );

		
	for (unsigned j=0; j<( m_FFTLength ); j++) 
	{
	    // perform thresholding
	    const double squaredBin = squaredModule( transfHammingWindowRe[ j ], transfHammingWindowIm[ j ]);
	    if (squaredBin <= squareThreshold) continue;
		
	    // Insert non-zero position indexes, doubled because they are floats
	    m_sparseKernelIs.push_back(j);
	    m_sparseKernelJs.push_back(k);

	    // take conjugate, normalise and add to array sparkernel
	    m_sparseKernelRealValues.push_back( transfHammingWindowRe[ j ]/m_FFTLength);
	    m_sparseKernelImagValues.push_back(-transfHammingWindowIm[ j ]/m_FFTLength);
	}

    }

    delete [] hammingWindowRe;
    delete [] hammingWindowIm;
    delete [] transfHammingWindowRe;
    delete [] transfHammingWindowIm;

}

//-----------------------------------------------------------------------------
double* ConstantQ::process( const double* fftdata )
{
    for (unsigned row=0; row<2*m_uK; row++) 
    {
	m_CQdata[ row ] = 0;
	m_CQdata[ row+1 ] = 0;
    }
    const unsigned *fftbin = &(m_sparseKernelIs[0]);
    const unsigned *cqbin  = &(m_sparseKernelJs[0]);
    const double   *real   = &(m_sparseKernelRealValues[0]);
    const double   *imag   = &(m_sparseKernelImagValues[0]);
    const unsigned int sparseCells = m_sparseKernelRealValues.size();
	
    for (unsigned i = 0; i<sparseCells; i++)
    {
	const unsigned row = cqbin[i];
	const unsigned col = fftbin[i];
	const double & r1  = real[i];
	const double & i1  = imag[i];
	const double & r2  = fftdata[ (2*m_FFTLength) - 2*col];
	const double & i2  = fftdata[ (2*m_FFTLength) - 2*col+1];
	// add the multiplication
	m_CQdata[ 2*row  ] += (r1*r2 - i1*i2);
	m_CQdata[ 2*row+1] += (r1*i2 + i1*r2);
    }

    return m_CQdata;
}


void ConstantQ::initialise( CQConfig Config )
{
    m_FS = Config.FS;
    m_FMin = Config.min;		// min freq
    m_FMax = Config.max;		// max freq
    m_BPO = Config.BPO;		// bins per octave
    m_CQThresh = Config.CQThresh;// ConstantQ threshold for kernel generation

    m_dQ = 1/(pow(2,(1/(double)m_BPO))-1);	// Work out Q value for Filter bank
    m_uK = (unsigned int) ceil(m_BPO * log(m_FMax/m_FMin)/log(2.0));	// No. of constant Q bins

//    std::cerr << "ConstantQ::initialise: rate = " << m_FS << ", fmin = " << m_FMin << ", fmax = " << m_FMax << ", bpo = " << m_BPO << ", K = " << m_uK << ", Q = " << m_dQ << std::endl;

    // work out length of fft required for this constant Q Filter bank
    m_FFTLength = (int) pow(2, nextpow2(ceil( m_dQ*m_FS/m_FMin )));

    m_hop = m_FFTLength/8; // <------ hop size is window length divided by 32

//    std::cerr << "ConstantQ::initialise: -> fft length = " << m_FFTLength << ", hop = " << m_hop << std::endl;

    // allocate memory for cqdata
    m_CQdata = new double [2*m_uK];
}

void ConstantQ::deInitialise()
{
    delete [] m_CQdata;
}

void ConstantQ::process(const double *FFTRe, const double* FFTIm,
                        double *CQRe, double *CQIm)
{
    for (unsigned row=0; row<m_uK; row++) 
    {
	CQRe[ row ] = 0;
	CQIm[ row ] = 0;
    }

    const unsigned *fftbin = &(m_sparseKernelIs[0]);
    const unsigned *cqbin  = &(m_sparseKernelJs[0]);
    const double   *real   = &(m_sparseKernelRealValues[0]);
    const double   *imag   = &(m_sparseKernelImagValues[0]);
    const unsigned int sparseCells = m_sparseKernelRealValues.size();
	
    for (unsigned i = 0; i<sparseCells; i++)
    {
	const unsigned row = cqbin[i];
	const unsigned col = fftbin[i];
	const double & r1  = real[i];
	const double & i1  = imag[i];
	const double & r2  = FFTRe[ m_FFTLength - col];
	const double & i2  = FFTIm[ m_FFTLength - col];
	// add the multiplication
	CQRe[ row ] += (r1*r2 - i1*i2);
	CQIm[ row ] += (r1*i2 + i1*r2);
    }
}
