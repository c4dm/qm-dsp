/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2006 Martin Gasser.
    All rights reserved.
*/

#ifndef _TCSGram_
#define _TCSGram_

#include <vector>
#include <valarray>
#include <utility>

#include "TonalEstimator.h"

typedef std::vector<std::pair<long, TCSVector> > vectorlist_t;

class TCSGram
{
public:	
	TCSGram();
	~TCSGram();
	void getTCSVector(int, TCSVector&) const;
	void addTCSVector(const TCSVector&);
	long getTime(size_t) const;
	long getDuration() const;
	void printDebug();
	int getSize() const { return m_VectorList.size(); }
	void reserve(size_t uSize) { m_VectorList.reserve(uSize); }
	void clear() { m_VectorList.clear(); }
	void setFrameDuration(const double dFrameDurationMS) { m_dFrameDurationMS = dFrameDurationMS; }
	void setNumBins(const unsigned int uNumBins) { m_uNumBins = uNumBins; }
	void normalize();
protected:
	vectorlist_t m_VectorList;
	unsigned int m_uNumBins;
	double m_dFrameDurationMS;
};

#endif
