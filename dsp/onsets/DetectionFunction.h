/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef DETECTIONFUNCTION_H
#define DETECTIONFUNCTION_H

#include "dsp/maths/MathUtilities.h"
#include "dsp/maths/MathAliases.h"
#include "dsp/phasevocoder/PhaseVocoder.h"
#include "base/Window.h"

#define DF_HFC (1)
#define DF_SPECDIFF (2)
#define DF_PHASEDEV (3)
#define DF_COMPLEXSD (4)
#define DF_BROADBAND (5)

struct DFConfig{
    double stepSecs; // DF step in seconds
    unsigned int stepSize; // DF step in samples
    unsigned int frameLength; // DF analysis window - usually 2*step
    int DFType; // type of detection function ( see defines )
    double dbRise; // only used for broadband df (and required for it)
};

class DetectionFunction  
{
public:
    double* getSpectrumMagnitude();
    DetectionFunction( DFConfig Config );
    virtual ~DetectionFunction();
    double process( double* TDomain );
    double process( double* magnitudes, double* phases );

private:
    double runDF();

    double HFC( unsigned int length, double* src);
    double specDiff( unsigned int length, double* src);
    double phaseDev(unsigned int length, double *srcMagnitude, double *srcPhase);
    double complexSD(unsigned int length, double *srcMagnitude, double *srcPhase);
    double broadband(unsigned int length, double *srcMagnitude, double *srcPhase);
	
private:
    void initialise( DFConfig Config );
    void deInitialise();

    int m_DFType;
    unsigned int m_dataLength;
    unsigned int m_halfLength;
    double m_dbRise;

    double* m_magHistory;
    double* m_phaseHistory;
    double* m_phaseHistoryOld;

    double* m_DFWindowedFrame; // Array for windowed analysis frame
    double* m_magnitude; // Magnitude of analysis frame ( frequency domain )
    double* m_thetaAngle;// Phase of analysis frame ( frequency domain )

    Window<double> *m_window;
    PhaseVocoder* m_phaseVoc;	// Phase Vocoder
};

#endif 
