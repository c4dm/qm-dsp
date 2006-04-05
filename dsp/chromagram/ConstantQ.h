/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef CONSTANTQ_H
#define CONSTANTQ_H

#include <vector>
#include "dsp/maths/MathAliases.h"
#include "dsp/maths/MathUtilities.h"

struct CQConfig{
    unsigned int FS;
    double min;
    double max;
    unsigned int BPO;
    double CQThresh;
};

class ConstantQ {
	
//public functions incl. sparsekernel so can keep out of loop in main
public:
    void process( double* FFTRe, double* FFTIm, double* CQRe, double* CQIm );

    ConstantQ( CQConfig Config );
    ~ConstantQ();

    double* process( double* FFTData );

    void sparsekernel();

    double hamming(int len, int n) {
	double out = 0.54 - 0.46*cos(2*PI*n/len);
	return(out);
    }
	
    int getnumwin() { return m_numWin;}
    double getQ() { return m_dQ;}
    int getK() {return m_uK ;}
    int getfftlength() { return m_FFTLength;}
    int gethop() { return m_hop;}

private:
    void initialise( CQConfig Config );
    void deInitialise();
	
    double* m_CQdata;
    unsigned int m_FS;
    double m_FMin;
    double m_FMax;
    double m_dQ;
    double m_CQThresh;
    unsigned int m_numWin;
    unsigned int m_hop;
    unsigned int m_BPO;
    unsigned int m_FFTLength;
    unsigned int m_uK;
    std::vector<unsigned> m_sparseKernelIs;
    std::vector<unsigned> m_sparseKernelJs;
    std::vector<double> m_sparseKernelImagValues;
    std::vector<double> m_sparseKernelRealValues;
};


#endif//CONSTANTQ_H

