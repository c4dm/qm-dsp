/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2006 Martin Gasser.
    All rights reserved.
*/

#ifndef _CHANGEDETECTIONFUNCTION_
#define _CHANGEDETECTIONFUNCTION_

#define DEBUG_CHANGE_DETECTION_FUNCTION 1

#include "TCSgram.h"

#include <valarray>
using std::valarray;

typedef	valarray<double> ChangeDistance;

struct ChangeDFConfig
{
	int smoothingWidth;
};

class ChangeDetectionFunction
{
public:
	ChangeDetectionFunction(ChangeDFConfig);
	~ChangeDetectionFunction();
	ChangeDistance process(const TCSGram& rTCSGram);
private:
	void setFilterWidth(const int iWidth);
	
private:
	valarray<double> m_vaGaussian;
	double m_dFilterSigma;
	int m_iFilterWidth;
};

#endif // _CHANGDETECTIONFUNCTION_
