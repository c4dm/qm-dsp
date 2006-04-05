/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/


#include "ChromaProcess.h"
#include "dsp/maths/Histogram.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChromaProcess::ChromaProcess()
{

}

ChromaProcess::~ChromaProcess()
{

}

int ChromaProcess::findChromaBias( vector<double> chromaVector, unsigned int BPO, unsigned int frames )
{
    vector<double> newChroma;
    vector<int> peakIndex;
    vector<int> modPeakIndex;

    unsigned int chromaLength = chromaVector.size(); 

    unsigned int newLength = chromaLength + (2*BPO);

    newChroma.resize( newLength );
    newChroma.clear();

    modPeakIndex.resize( newLength );
    modPeakIndex.clear();

    //adds last row at the top and first row at the bottom to create 
    //circularity - effectively adds 2 to the bpo-length of the vectors:

    for( unsigned int i = 0; i < BPO; i++ )
    {
	newChroma.push_back( chromaVector[ chromaLength - BPO + i ] );
    }

    for( unsigned i = 0; i < chromaLength; i++ )
    {
	newChroma.push_back( chromaVector[ i ] );
    }

    for( unsigned i = 0; i < BPO; i++ )
    {
	newChroma.push_back( chromaVector[ i ] );
    }

    // pick peaks in the chroma
    peakIndex = getPeaks( newChroma, BPO );

    // modularises to res = bpo/12 bins:
    // corrects the mod value for bin 3
    modPeakIndex = mod( peakIndex, 3 );

    // finds the highest concentration of peaks on the bpo/12 bin resolution
    THistogram<int> m_hist(3);

    double ave, adev, sdev, var, skew, ccurt;

    m_hist.compute(modPeakIndex);

    m_hist.getMoments( modPeakIndex, ave, adev, sdev, var, skew, ccurt );

    vector <double> histogram = m_hist.geTHistogramD();
    //////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////
	// Find actual bias from histogram
	int minIdx, maxIdx;
	double min, max;

	findHistMaxMin( histogram, &max, &maxIdx, &min, &minIdx );

/*
  FILE* foutchroma = fopen("../testdata/newchroma.bin","wb");
  FILE* foutpeaks = fopen("../testdata/peaks.bin","wb");


  fwrite( &chromaVector[0], sizeof(double), chromaVector.size(), foutchroma );
  fwrite( &histogram[0], sizeof(double), histogram.size(), foutpeaks );

  fclose( foutchroma );
  fclose( foutpeaks );
*/
	return maxIdx - 1;
}


vector <int> ChromaProcess::getPeaks(vector <double> chroma, unsigned int BPO)
{
    vector <int> peaks;
	
    double pre = 0;
    double post = 0;
    double current = 0;

    unsigned int BPOCounter = 0;
    unsigned int mult = 0;
    unsigned int idx = 0;

    for( unsigned int i = 0; i < chroma.size() - 0; i++ )
    {
	BPOCounter++;

	pre = chroma[ i ];
	current = chroma[ i + 1 ];
	post = chroma[ i + 2 ];
		
	if( (current > 0) && (current > pre) && (current > post) )
	{
	    peaks.push_back( BPOCounter + 1);
	}

		
	if( BPOCounter == (BPO - 2 ) )
	{
	    BPOCounter = 0;
	    i+=2;
	}
		
    }

    /*
      for( unsigned int i = 1; i < chroma.size() - 1; i++ )
      {
      BPOCounter++ ;

      pre = chroma[ i - 1 ];
      current = chroma[ i ];
      post = chroma[ i + 1 ];
		
      if( (current > 0) && (current > pre) && (current > post) )
      {
      peaks.push_back( BPOCounter + 1 );
      }

      if( BPOCounter == (PO - 1) )
      {
      BPOCounter = 1;
      i+=2;
      }
      }
    */
    return peaks;
}

vector <int> ChromaProcess::mod(vector <int> input, int res)
{
    vector <int> result;

    for( unsigned int i = 0; i < input.size(); i++ )
    {
	int val = input[ i ];
	int res = val - res * floor( (double)val / (double)res );

	if( val != 0 )
	{
	    if( res == 0 )
		res = 3;

	    result.push_back( res );
	}
	else
	{
	    result.push_back( val );
	}
    }
    return result;
}

void ChromaProcess::findHistMaxMin( vector<double> hist, double* max, int* maxIdx, double* min, int* minIdx )
{
    double temp = 0.0;
    unsigned int vecLength = hist.size();

    *minIdx = 0;
    *maxIdx = 0;

    *min = hist[0];
    *max = *min;

    for( unsigned int u = 0; u < vecLength; u++ )
    {
	temp = hist[ u ];

	if( temp < *min )
	{
	    *min =  temp ;
	    *minIdx = u;
	}
	if( temp > *max )
	{
	    *max =  temp ;
	    *maxIdx = u;
	}

    }
}
