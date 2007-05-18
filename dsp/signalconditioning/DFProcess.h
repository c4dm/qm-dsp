/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef CDFPROCESS_H
#define CDFPROCESS_H

#include <stdio.h>
#include "FiltFilt.h"

struct DFProcConfig{
    unsigned int length; 
    unsigned int LPOrd; 
    double *LPACoeffs; 
    double *LPBCoeffs; 
    unsigned int winPre;
    unsigned int winPost; 
    double AlphaNormParam;
    bool isMedianPositive;
};

class DFProcess  
{
public:
    DFProcess( DFProcConfig Config );
    virtual ~DFProcess();

    void process( double* src, double* dst );

	
private:
    void initialise( DFProcConfig Config );
    void deInitialise();
    void removeDCNormalize( double *src, double*dst );
    void medianFilter( double* src, double* dst );

    unsigned int m_length;
    unsigned int m_FFOrd;

    unsigned int m_winPre;
    unsigned int m_winPost;

    double m_alphaNormParam;

    double* filtSrc;
    double* filtDst;

    double* m_filtScratchIn;
    double* m_filtScratchOut;

    FiltFiltConfig m_FilterConfigParams;

    FiltFilt* m_FiltFilt;

    bool m_isMedianPositive;
};

#endif
