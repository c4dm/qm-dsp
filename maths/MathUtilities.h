/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef MATHUTILITIES_H
#define MATHUTILITIES_H

#include <vector>

class MathUtilities  
{
public:	
    static double round( double x );
    static void	  getFrameMinMax( const double* data, unsigned int len,  double* min, double* max );
    static double mean( const double* src, unsigned int len );
    static double sum( const double* src, unsigned int len );
    static double princarg( double ang );
    static double median( const double* src, unsigned int len );
    static double mod( double x, double y);
    static void	  getAlphaNorm(const double *data, unsigned int len, unsigned int alpha, double* ANorm);
    static double getAlphaNorm(const std::vector <double> &data, unsigned int alpha );
    static void   circShift( double* data, int length, int shift);
    static int	  getMax( double* data, unsigned int length, double* max );
    static int    compareInt(const void * a, const void * b);
};

#endif
