/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef FILTER_H
#define FILTER_H

#ifndef NULL
#define NULL 0
#endif

struct FilterConfig{
    unsigned int ord;
    double* ACoeffs;
    double* BCoeffs;
};

class Filter  
{
public:
    Filter( FilterConfig Config );
    virtual ~Filter();

    void reset();

    void process( double *src, double *dst, unsigned int length );
	

private:
    void initialise( FilterConfig Config );
    void deInitialise();

    unsigned int m_ord;

    double* m_inBuffer;
    double* m_outBuffer;

    double* m_ACoeffs;
    double* m_BCoeffs;
};

#endif
