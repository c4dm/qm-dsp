/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#include "MathUtilities.h"

#include <iostream>
#include <cmath>


double MathUtilities::mod(double x, double y)
{
    double a = floor( x / y );

    double b = x - ( y * a );
    return b;
}

double MathUtilities::princarg(double ang)
{
    double ValOut;

    ValOut = mod( ang + M_PI, -2 * M_PI ) + M_PI;

    return ValOut;
}

void MathUtilities::getAlphaNorm(const double *data, unsigned int len, unsigned int alpha, double* ANorm)
{
    unsigned int i;
    double temp = 0.0;
    double a=0.0;
	
    for( i = 0; i < len; i++)
    {
	temp = data[ i ];
		
	a  += ::pow( fabs(temp), alpha );
    }
    a /= ( double )len;
    a = ::pow( a, ( 1.0 / (double) alpha ) );

    *ANorm = a;
}

double MathUtilities::getAlphaNorm( const std::vector <double> &data, unsigned int alpha )
{
    unsigned int i;
    unsigned int len = data.size();
    double temp = 0.0;
    double a=0.0;
	
    for( i = 0; i < len; i++)
    {
	temp = data[ i ];
		
	a  += ::pow( fabs(temp), alpha );
    }
    a /= ( double )len;
    a = ::pow( a, ( 1.0 / (double) alpha ) );

    return a;
}

double MathUtilities::round(double x)
{
    double val = (double)floor(x + 0.5);
  
    return val;
}

double MathUtilities::median(const double *src, unsigned int len)
{
    unsigned int i, j;
    double tmp = 0.0;
    double tempMedian;
    double medianVal;
 
    double* scratch = new double[ len ];//Vector < double > sortedX = Vector < double > ( size );

    for ( i = 0; i < len; i++ )
    {
	scratch[i] = src[i];
    }

    for ( i = 0; i < len - 1; i++ )
    {
	for ( j = 0; j < len - 1 - i; j++ )
	{
	    if ( scratch[j + 1] < scratch[j] )
	    {
		// compare the two neighbors
		tmp = scratch[j]; // swap a[j] and a[j+1]
		scratch[j] = scratch[j + 1];
		scratch[j + 1] = tmp;
	    }
	}
    }
    int middle;
    if ( len % 2 == 0 )
    {
	middle = len / 2;
	tempMedian = ( scratch[middle] + scratch[middle - 1] ) / 2;
    }
    else
    {
	middle = ( int )floor( len / 2.0 );
	tempMedian = scratch[middle];
    }

    medianVal = tempMedian;

    delete [] scratch;
    return medianVal;
}

double MathUtilities::sum(const double *src, unsigned int len)
{
    unsigned int i ;
    double retVal =0.0;

    for(  i = 0; i < len; i++)
    {
	retVal += src[ i ];
    }

    return retVal;
}

double MathUtilities::mean(const double *src, unsigned int len)
{
    double retVal =0.0;

    double s = sum( src, len );
	
    retVal =  s  / (double)len;

    return retVal;
}

void MathUtilities::getFrameMinMax(const double *data, unsigned int len, double *min, double *max)
{
    unsigned int i;
    double temp = 0.0;
    double a=0.0;
	
    *min = data[0];
    *max = data[0];

    for( i = 0; i < len; i++)
    {
	temp = data[ i ];

	if( temp < *min )
	{
	    *min =  temp ;
	}
	if( temp > *max )
	{
	    *max =  temp ;
	}
		
    }
}

int MathUtilities::getMax( double* pData, unsigned int Length, double* pMax )
{
	unsigned int index = 0;
	unsigned int i;
	double temp = 0.0;
	
	double max = pData[0];

	for( i = 0; i < Length; i++)
	{
		temp = pData[ i ];

		if( temp > max )
		{
			max =  temp ;
			index = i;
		}
		
   	}

	*pMax = max;


	return index;
}

void MathUtilities::circShift( double* pData, int length, int shift)
{
	shift = shift % length;
	double temp;
	int i,n;

	for( i = 0; i < shift; i++)
	{
		temp=*(pData + length - 1);

		for( n = length-2; n >= 0; n--)
		{
			*(pData+n+1)=*(pData+n);
		}

        *pData = temp;
    }
}

int MathUtilities::compareInt (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

