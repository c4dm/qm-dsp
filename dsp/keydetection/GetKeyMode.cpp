// GetKeyMode.cpp: implementation of the CGetKeyMode class.
//
//////////////////////////////////////////////////////////////////////

#include "GetKeyMode.h"
#include "dsp/maths/MathUtilities.h"
#include "base/Pitch.h"

#include <iostream>

// Chords profile
static double MajProfile[36] = 
{ 0.0384, 0.0629, 0.0258, 0.0121, 0.0146, 0.0106, 0.0364, 0.0610, 0.0267,
  0.0126, 0.0121, 0.0086, 0.0364, 0.0623, 0.0279, 0.0275, 0.0414, 0.0186,
  0.0173, 0.0248, 0.0145, 0.0364, 0.0631, 0.0262, 0.0129, 0.0150, 0.0098,
  0.0312, 0.0521, 0.0235, 0.0129, 0.0142, 0.0095, 0.0289, 0.0478, 0.0239};

static double MinProfile[36] =
{ 0.0375, 0.0682, 0.0299, 0.0119, 0.0138, 0.0093, 0.0296, 0.0543, 0.0257,
  0.0292, 0.0519, 0.0246, 0.0159, 0.0234, 0.0135, 0.0291, 0.0544, 0.0248,
  0.0137, 0.0176, 0.0104, 0.0352, 0.0670, 0.0302, 0.0222, 0.0349, 0.0164,
  0.0174, 0.0297, 0.0166, 0.0222, 0.0401, 0.0202, 0.0175, 0.0270, 0.0146};
//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GetKeyMode::GetKeyMode( int sampleRate, float tuningFrequency,
			double hpcpAverage, double medianAverage )
:
m_hpcpAverage( hpcpAverage ),
m_medianAverage( medianAverage ),
m_ChrPointer(0),
m_DecimatedBuffer(0),
m_ChromaBuffer(0),
m_MeanHPCP(0),
m_MajCorr(0),
m_MinCorr(0),
m_Keys(0),
m_MedianFilterBuffer(0),
m_SortedBuffer(0)
{
	m_DecimationFactor = 8;

	//Chromagram configuration parameters
	m_ChromaConfig.isNormalised = 1;
	m_ChromaConfig.FS = lrint(sampleRate/(double)m_DecimationFactor);

//	m_ChromaConfig.min = 111.0641;
//	m_ChromaConfig.max = 1.7770e+003;

//	m_ChromaConfig.min = Pitch::getFrequencyForPitch
//		(12, 0, tuningFrequency);
//	m_ChromaConfig.max = Pitch::getFrequencyForPitch
//		(96, 0, tuningFrequency);

	// The chromagram minimum pitch is 1/6 of a tone above A, two
	// octaves below middle C (for a 36-bin chromagram).  The
	// maximum pitch is four octaves higher.

	m_ChromaConfig.min = Pitch::getFrequencyForPitch
		(45, 1.f / 3.f, tuningFrequency);

	m_ChromaConfig.max = m_ChromaConfig.min * 2;
	m_ChromaConfig.max = m_ChromaConfig.max * 2;
	m_ChromaConfig.max = m_ChromaConfig.max * 2;
	m_ChromaConfig.max = m_ChromaConfig.max * 2;

	std::cerr << "Chromagram range: " << m_ChromaConfig.min << " -> " << m_ChromaConfig.max << std::endl;

	m_ChromaConfig.BPO = 36;
	m_ChromaConfig.CQThresh = 0.0054;

	//Chromagram inst.
	m_Chroma = new Chromagram( m_ChromaConfig );

	//Get calculated parameters from chroma object
	m_ChromaFrameSize = m_Chroma->getFrameSize();
	//override hopsize for this application
	m_ChromaHopSize = m_ChromaFrameSize;//m_Chroma->GetHopSize();
	m_BPO = m_ChromaConfig.BPO;

	//Chromagram average and estimated key median filter lengths
	m_ChromaBuffersize = (int)ceil( m_hpcpAverage * m_ChromaConfig.FS/m_ChromaFrameSize );
	m_MedianWinsize = (int)ceil( m_medianAverage * m_ChromaConfig.FS/m_ChromaFrameSize );

	//Reset counters
	m_bufferindex = 0;
	m_ChromaBufferFilling = 0;
	m_MedianBufferFilling = 0;


	//Spawn objectc/arrays
	m_DecimatedBuffer = new double[m_ChromaFrameSize];

	m_ChromaBuffer = new double[m_BPO * m_ChromaBuffersize];
	memset( m_ChromaBuffer, 0, sizeof(double) * m_BPO * m_ChromaBuffersize);
	
	m_MeanHPCP = new double[m_BPO];

	m_MajCorr = new double[m_BPO];
	m_MinCorr = new double[m_BPO];
	m_Keys  = new double[2*m_BPO];
	
	m_MedianFilterBuffer = new int[ m_MedianWinsize ];
	memset( m_MedianFilterBuffer, 0, sizeof(int)*m_MedianWinsize);
	
	m_SortedBuffer = new int[ m_MedianWinsize ];
	memset( m_SortedBuffer, 0, sizeof(int)*m_MedianWinsize);	

	m_Decimator = new Decimator( m_ChromaFrameSize*m_DecimationFactor, m_DecimationFactor );
}

GetKeyMode::~GetKeyMode()
{

	delete m_Chroma;
	delete m_Decimator;

	delete [] m_DecimatedBuffer;
	delete [] m_ChromaBuffer;
	delete [] m_MeanHPCP;
	delete [] m_MajCorr;
	delete [] m_MinCorr;
	delete [] m_Keys;
	delete [] m_MedianFilterBuffer;
	delete [] m_SortedBuffer;
}

double GetKeyMode::krumCorr(double *pData1, double *pData2, unsigned int length)
{
	double retVal= 0.0;
		
	double num = 0;
	double den = 0;
	double mX = MathUtilities::mean( pData1, length );
	double mY = MathUtilities::mean( pData2, length );

	double sum1 = 0;
	double sum2 = 0;

	for( unsigned int i = 0; i <length; i++ )
	{
		num += ( pData1[i] - mX ) * ( pData2[i] - mY );

		sum1 += ( (pData1[i]-mX) * (pData1[i]-mX) );
		sum2 += ( (pData2[i]-mY) * (pData2[i]-mY) );
	}
	
	den = sqrt(sum1 * sum2);
	
	if( den>0 )
		retVal = num/den;
	else
		retVal = 0;


	return retVal;
}

int GetKeyMode::process(double *PCMData)
{
	int key;

	unsigned int j,k;

	//////////////////////////////////////////////
	m_Decimator->process( PCMData, m_DecimatedBuffer);

	m_ChrPointer = m_Chroma->process( m_DecimatedBuffer );		

	std::cout << "raw chroma: ";
	for (int ii = 0; ii < m_BPO; ++ii) {
		std::cout << m_ChrPointer[ii] << " ";
	}
	std::cout << std::endl;

	// populate hpcp values;
	int cbidx;
	for( j = 0; j < m_BPO; j++ )
	{
		cbidx = (m_bufferindex * m_BPO) + j;
		m_ChromaBuffer[ cbidx ] = m_ChrPointer[j];
	}

	//keep track of input buffers;
	if( m_bufferindex++ >= m_ChromaBuffersize - 1) 
		m_bufferindex = 0;

	// track filling of chroma matrix
	if( m_ChromaBufferFilling++ >= m_ChromaBuffersize)
		m_ChromaBufferFilling = m_ChromaBuffersize;

	//calculate mean 		
	for( k = 0; k < m_BPO; k++ )
	{
		double mnVal = 0.0;
		for( j = 0; j < m_ChromaBufferFilling; j++ )
		{
			mnVal += m_ChromaBuffer[ k + (j*m_BPO) ];
		}

		m_MeanHPCP[k] = mnVal/(double)m_ChromaBufferFilling;
	}


	for( k = 0; k < m_BPO; k++ )
	{
		m_MajCorr[k] = krumCorr( m_MeanHPCP, MajProfile, m_BPO );
		m_MinCorr[k] = krumCorr( m_MeanHPCP, MinProfile, m_BPO );

		MathUtilities::circShift( MajProfile, m_BPO, 1 );
		MathUtilities::circShift( MinProfile, m_BPO, 1 );
	}

	for( k = 0; k < m_BPO; k++ )
	{
		m_Keys[k] = m_MajCorr[k];
		m_Keys[k+m_BPO] = m_MinCorr[k];
	}
/*
	std::cout << "raw keys: ";
	for (int ii = 0; ii < 2*m_BPO; ++ii) {
		std::cout << m_Keys[ii] << " ";
	}
	std::cout << std::endl;
*/
	double dummy;
	key = /*1 +*/ (int)ceil( (double)MathUtilities::getMax( m_Keys, 2* m_BPO, &dummy )/3 );

//	std::cout << "key pre-sorting: " << key << std::endl;


	//Median filtering

	// track Median buffer initial filling
	if( m_MedianBufferFilling++ >= m_MedianWinsize)
		m_MedianBufferFilling = m_MedianWinsize;
		
	//shift median buffer
	for( k = 1; k < m_MedianWinsize; k++ )
	{
		m_MedianFilterBuffer[ k - 1 ] = m_MedianFilterBuffer[ k ];
	}

	//write new key value into median buffer
	m_MedianFilterBuffer[ m_MedianWinsize - 1 ] = key;


	//Copy median into sorting buffer, reversed
	unsigned int ijx = 0;
	for( k = 0; k < m_MedianWinsize; k++ )
	{
		m_SortedBuffer[k] = m_MedianFilterBuffer[m_MedianWinsize-1-ijx];
		ijx++;
	}


	//quicksort 
	qsort(m_SortedBuffer, m_MedianBufferFilling, sizeof(unsigned int), MathUtilities::compareInt);
/*
	std::cout << "sorted: ";
	for (int ii = 0; ii < m_MedianBufferFilling; ++ii) {
		std::cout << m_SortedBuffer[ii] << " ";
	}
	std::cout << std::endl;
*/
	int sortlength = m_MedianBufferFilling;
	int midpoint = (int)ceil((double)sortlength/2);

	if( midpoint <= 0 )
		midpoint = 1;

	key = m_SortedBuffer[midpoint-1];

	return key;
}


int GetKeyMode::isModeMinor( int key )
{ 
//	return ((key-1 - (int)MathUtilities::mod((double)(key-1),(double)12))/12); 
	return (key > 12);
}
