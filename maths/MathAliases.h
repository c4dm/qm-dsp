/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright 2005-2006 Christian Landone.
    All rights reserved.
*/

#ifndef MATHALIASES_H
#define MATHALIASES_H

#include <cmath>
#include <complex>

using namespace std;
typedef complex<double> ComplexData;


#ifndef PI
#define PI (3.14159265358979232846)
#endif

#define TWO_PI 		(*2.PI)

#define EPS 2.2204e-016

/* aliases to math.h functions */
#define EXP				exp
#define COS				cos
#define SIN				sin
#define ABS				fabs
#define POW				powf
#define SQRT			sqrtf
#define LOG10			log10f
#define LOG				logf
#define FLOOR			floorf
#define TRUNC			truncf

/* aliases to complex.h functions */
/** sample = EXPC(complex) */
#define EXPC			cexpf
/** complex = CEXPC(complex) */
#define CEXPC			cexp
/** sample = ARGC(complex) */
#define ARGC			cargf
/** sample = ABSC(complex) norm */
#define ABSC			cabsf
/** sample = REAL(complex) */
#define REAL			crealf
/** sample = IMAG(complex) */
#define IMAG			cimagf

#endif
