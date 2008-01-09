/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#include "Correlation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Correlation::Correlation()
{

}

Correlation::~Correlation()
{

}

void Correlation::doAutoUnBiased(double *src, double *dst, unsigned int length)
{
    double tmp = 0.0;
    double outVal = 0.0;

    unsigned int i,j;

    for( i = 0; i <  length; i++)
    {
	for( j = i; j < length; j++)
	{
	    tmp += src[ j-i ] * src[ j ]; 
	}


	outVal = tmp / ( length - i );

	if( outVal <= 0 )
	    dst[ i ] = EPS;
	else
	    dst[ i ] = outVal;

	tmp = 0.0;
    }
}
