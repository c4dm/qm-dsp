/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */
/*
    Copyright (c) 2005 Centre for Digital Music ( C4DM )
                       Queen Mary Univesrity of London

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.  See the file
    COPYING included with this distribution for more information.
 */

#ifndef QM_DSP_GETKEYMODE_H
#define QM_DSP_GETKEYMODE_H


#include "dsp/rateconversion/Decimator.h"
#include "dsp/chromagram/Chromagram.h"


class GetKeyMode  
{
public:
    GetKeyMode( int sampleRate, float tuningFrequency,
                double hpcpAverage, double medianAverage );

    virtual ~GetKeyMode();

    int process( double* PCMData );

    double krumCorr( const double *pDataNorm, const double *pProfileNorm, 
                     int shiftProfile, int length );

    int getBlockSize() { return m_chromaFrameSize * m_decimationFactor; }
    int getHopSize() { return m_chromaHopSize * m_decimationFactor; }

    double* getChroma() { return m_chrPointer; }
    int getChromaSize();

    double* getMeanHPCP() { return m_meanHPCP; }

    double* getKeyStrengths();

    bool isModeMinor( int key ); 

protected:

    double m_hpcpAverage;
    double m_medianAverage;
    int m_decimationFactor;

    // Decimator (fixed)
    Decimator* m_decimator;

    // Chroma configuration
    ChromaConfig m_chromaConfig;

    // Chromagram object
    Chromagram* m_chroma;

    // Chromagram output pointer
    double* m_chrPointer;

    // Framesize
    int m_chromaFrameSize;

    // Hop
    int m_chromaHopSize;

    int m_chromaBufferSize;
    int m_medianWinSize;
        
    int m_bufferIndex;
    int m_chromaBufferFilling;
    int m_medianBufferFilling;

    double* m_decimatedBuffer;
    double* m_chromaBuffer;
    double* m_meanHPCP;

    double* m_majProfileNorm;
    double* m_minProfileNorm;
    double* m_majCorr;
    double* m_minCorr;
    int* m_medianFilterBuffer;
    int* m_sortedBuffer;

    double *m_keyStrengths;
};

#endif // !defined QM_DSP_GETKEYMODE_H
