/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef CORRELATION_H
#define CORRELATION_H

#define  EPS  2.2204e-016

class Correlation  
{
public:
    void doAutoUnBiased( double* src, double* dst, unsigned int length );
    Correlation();
    virtual ~Correlation();

};

#endif // 
