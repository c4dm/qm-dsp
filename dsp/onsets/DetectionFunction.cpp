/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#include "DetectionFunction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DetectionFunction::DetectionFunction( DFConfig Config ) :
    m_window(0)
{
    magHistory = NULL;
    phaseHistory = NULL;
    phaseHistoryOld = NULL;
    j = ComplexData( 0, 1 );

    initialise( Config );
}

DetectionFunction::~DetectionFunction()
{
    deInitialise();
}


void DetectionFunction::initialise( DFConfig Config )
{
    m_dataLength = Config.frameLength;
    m_halfLength = m_dataLength/2;
    m_DFType = Config.DFType;

    magHistory = new double[ m_halfLength ];
    memset(magHistory,0, m_halfLength*sizeof(double));
		
    phaseHistory = new double[ m_halfLength ];
    memset(phaseHistory,0, m_halfLength*sizeof(double));

    phaseHistoryOld = new double[ m_halfLength ];
    memset(phaseHistoryOld,0, m_halfLength*sizeof(double));

    m_phaseVoc = new PhaseVocoder;

    m_DFWindowedFrame = new double[ m_dataLength ];
    m_magnitude = new double[ m_halfLength ];
    m_thetaAngle = new double[ m_halfLength ];

    m_window = new Window<double>(HanningWindow, m_dataLength);
}

void DetectionFunction::deInitialise()
{
    delete [] magHistory ;
    delete [] phaseHistory ;
    delete [] phaseHistoryOld ;

    delete m_phaseVoc;

    delete [] m_DFWindowedFrame;
    delete [] m_magnitude;
    delete [] m_thetaAngle;

    delete m_window;
}

double DetectionFunction::process( double *TDomain )
{
    double retVal = 0;

    m_window->cut( TDomain, m_DFWindowedFrame );
	
    m_phaseVoc->process( m_dataLength, m_DFWindowedFrame, m_magnitude, m_thetaAngle );

    switch( m_DFType )
    {
    case DF_HFC:
	retVal = HFC( m_halfLength, m_magnitude);
	break;
	
    case  DF_SPECDIFF:
	retVal = specDiff( m_halfLength, m_magnitude);
	break;
	
    case DF_PHASEDEV:
	retVal = phaseDev( m_halfLength, m_magnitude, m_thetaAngle);
	break;
	
    case DF_COMPLEXSD:
	retVal = complexSD( m_halfLength, m_magnitude, m_thetaAngle);
	break;
    }
	
    return retVal;
}

double DetectionFunction::HFC(unsigned int length, double *src)
{
    unsigned int i;
    double val = 0;

    for( i = 0; i < length; i++)
    {
	val += src[ i ] * ( i + 1);
    }
    return val;
}

double DetectionFunction::specDiff(unsigned int length, double *src)
{
    unsigned int i;
    double val = 0.0;
    double temp = 0.0;
    double diff = 0.0;

    for( i = 0; i < length; i++)
    {
	temp = fabs( (src[ i ] * src[ i ]) - (magHistory[ i ] * magHistory[ i ]) );
		
	diff= sqrt(temp);

	if( src[ i ] > 0.1)
	{
	    val += diff;
	}

	magHistory[ i ] = src[ i ];
    }

    return val;
}


double DetectionFunction::phaseDev(unsigned int length, double *srcMagnitude, double *srcPhase)
{
    unsigned int i;
    double tmpPhase = 0;
    double tmpVal = 0;
    double val = 0;

    double dev = 0;

    for( i = 0; i < length; i++)
    {
	tmpPhase = (srcPhase[ i ]- 2*phaseHistory[ i ]+phaseHistoryOld[ i ]);
	dev = MathUtilities::princarg( tmpPhase );
		
	if( srcMagnitude[ i  ] > 0.1)
	{
	    tmpVal  = fabs( dev);
	    val += tmpVal ;
	}

	phaseHistoryOld[ i ] = phaseHistory[ i ] ;
	phaseHistory[ i ] = srcPhase[ i ];
    }
	
	
    return val;
}


double DetectionFunction::complexSD(unsigned int length, double *srcMagnitude, double *srcPhase)
{
    unsigned int i;
    double val = 0;
    double tmpPhase = 0;
    double tmpReal = 0;
    double tmpImag = 0;
   
    double dev = 0;
    ComplexData meas = ComplexData( 0, 0 );

    for( i = 0; i < length; i++)
    {
	tmpPhase = (srcPhase[ i ]- 2*phaseHistory[ i ]+phaseHistoryOld[ i ]);
	dev= MathUtilities::princarg( tmpPhase );
		
	meas = magHistory[i] - ( srcMagnitude[ i ] * exp( j * dev) );

	tmpReal = real( meas );
	tmpImag = imag( meas );

	val += sqrt( (tmpReal * tmpReal) + (tmpImag * tmpImag) );
		
	phaseHistoryOld[ i ] = phaseHistory[ i ] ;
	phaseHistory[ i ] = srcPhase[ i ];
	magHistory[ i ] = srcMagnitude[ i ];
    }

    return val;
}

double* DetectionFunction::getSpectrumMagnitude()
{
    return m_magnitude;
}

