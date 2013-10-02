/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "dsp/transforms/FFT.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestFFT)

#define COMPARE_CONST(a, n) \
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_SMALL(a[cmp_i] - n, 1e-14);				\
    }

#define COMPARE_ARRAY(a, b)						\
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_SMALL(a[cmp_i] - b[cmp_i], 1e-14);			\
    }

//!!! need at least one test with complex time-domain signal

BOOST_AUTO_TEST_CASE(forwardArrayBounds)
{
    // initialise bins to something recognisable, so we can tell
    // if they haven't been written
    double in[] = { 1, 1, -1, -1 };
    double re[] = { 999, 999, 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re+1, im+1);
    // And check we haven't overrun the arrays
    BOOST_CHECK_EQUAL(re[0], 999.0);
    BOOST_CHECK_EQUAL(im[0], 999.0);
    BOOST_CHECK_EQUAL(re[5], 999.0);
    BOOST_CHECK_EQUAL(im[5], 999.0);
}

BOOST_AUTO_TEST_CASE(r_forwardArrayBounds)
{
    // initialise bins to something recognisable, so we can tell
    // if they haven't been written
    double in[] = { 1, 1, -1, -1 };
    double re[] = { 999, 999, 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999, 999, 999 };
    FFTReal(4).forward(in, re+1, im+1);
    // And check we haven't overrun the arrays
    BOOST_CHECK_EQUAL(re[0], 999.0);
    BOOST_CHECK_EQUAL(im[0], 999.0);
    BOOST_CHECK_EQUAL(re[5], 999.0);
    BOOST_CHECK_EQUAL(im[5], 999.0);
}

BOOST_AUTO_TEST_CASE(inverseArrayBounds)
{
    // initialise bins to something recognisable, so we can tell
    // if they haven't been written
    double re[] = { 0, 1, 0, 1 };
    double im[] = { 0, -2, 0, 2 };
    double outre[] = { 999, 999, 999, 999, 999, 999 };
    double outim[] = { 999, 999, 999, 999, 999, 999 };
    FFT(4).process(true, re, im, outre+1, outim+1);
    // And check we haven't overrun the arrays
    BOOST_CHECK_EQUAL(outre[0], 999.0);
    BOOST_CHECK_EQUAL(outim[0], 999.0);
    BOOST_CHECK_EQUAL(outre[5], 999.0);
    BOOST_CHECK_EQUAL(outim[5], 999.0);
}

BOOST_AUTO_TEST_CASE(r_inverseArrayBounds)
{
    // initialise bins to something recognisable, so we can tell
    // if they haven't been written
    double re[] = { 0, 1, 0 };
    double im[] = { 0, -2, 0 };
    double outre[] = { 999, 999, 999, 999, 999, 999 };
    FFTReal(4).inverse(re, im, outre+1);
    // And check we haven't overrun the arrays
    BOOST_CHECK_EQUAL(outre[0], 999.0);
    BOOST_CHECK_EQUAL(outre[5], 999.0);
}

BOOST_AUTO_TEST_CASE(dc)
{
    // DC-only signal. The DC bin is purely real
    double in[] = { 1, 1, 1, 1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 4.0);
    BOOST_CHECK_EQUAL(re[1], 0.0);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_EQUAL(re[3], 0.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
    COMPARE_CONST(backim, 0.0);
}

BOOST_AUTO_TEST_CASE(r_dc)
{
    // DC-only signal. The DC bin is purely real
    double in[] = { 1, 1, 1, 1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFTReal(4).forward(in, re, im);
    BOOST_CHECK_EQUAL(re[0], 4.0);
    BOOST_CHECK_EQUAL(re[1], 0.0);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_EQUAL(re[3], 0.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    // check conjugates are reconstructed
    re[3] = 999;
    im[3] = 999;
    FFTReal(4).inverse(re, im, back);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(sine)
{
    // Sine. Output is purely imaginary
    double in[] = { 0, 1, 0, -1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re, im);
    COMPARE_CONST(re, 0.0);
    BOOST_CHECK_EQUAL(im[0], 0.0);
    BOOST_CHECK_EQUAL(im[1], -2.0);
    BOOST_CHECK_EQUAL(im[2], 0.0);
    BOOST_CHECK_EQUAL(im[3], 2.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
    COMPARE_CONST(backim, 0.0);
}

BOOST_AUTO_TEST_CASE(r_sine)
{
    // Sine. Output is purely imaginary
    double in[] = { 0, 1, 0, -1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFTReal(4).forward(in, re, im);
    COMPARE_CONST(re, 0.0);
    BOOST_CHECK_EQUAL(im[0], 0.0);
    BOOST_CHECK_EQUAL(im[1], -2.0);
    BOOST_CHECK_EQUAL(im[2], 0.0);
    BOOST_CHECK_EQUAL(im[3], 2.0);
    double back[4];
    // check conjugates are reconstructed
    re[3] = 999;
    im[3] = 999;
    FFTReal(4).inverse(re, im, back);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(cosine)
{
    // Cosine. Output is purely real
    double in[] = { 1, 0, -1, 0 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_EQUAL(re[1], 2.0);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_EQUAL(re[3], 2.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
    COMPARE_CONST(backim, 0.0);
}

BOOST_AUTO_TEST_CASE(r_cosine)
{
    // Cosine. Output is purely real
    double in[] = { 1, 0, -1, 0 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFTReal(4).forward(in, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_EQUAL(re[1], 2.0);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_EQUAL(re[3], 2.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    // check conjugates are reconstructed
    re[3] = 999;
    im[3] = 999;
    FFTReal(4).inverse(re, im, back);
    COMPARE_ARRAY(back, in);
}
	
BOOST_AUTO_TEST_CASE(sineCosine)
{
    // Sine and cosine mixed
    double in[] = { 0.5, 1, -0.5, -1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_CLOSE(re[1], 1.0, 1e-12);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_CLOSE(re[3], 1.0, 1e-12);
    BOOST_CHECK_EQUAL(im[0], 0.0);
    BOOST_CHECK_CLOSE(im[1], -2.0, 1e-12);
    BOOST_CHECK_EQUAL(im[2], 0.0);
    BOOST_CHECK_CLOSE(im[3], 2.0, 1e-12);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
    COMPARE_CONST(backim, 0.0);
}
	
BOOST_AUTO_TEST_CASE(r_sineCosine)
{
    // Sine and cosine mixed
    double in[] = { 0.5, 1, -0.5, -1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFTReal(4).forward(in, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_CLOSE(re[1], 1.0, 1e-12);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_CLOSE(re[3], 1.0, 1e-12);
    BOOST_CHECK_EQUAL(im[0], 0.0);
    BOOST_CHECK_CLOSE(im[1], -2.0, 1e-12);
    BOOST_CHECK_EQUAL(im[2], 0.0);
    BOOST_CHECK_CLOSE(im[3], 2.0, 1e-12);
    double back[4];
    // check conjugates are reconstructed
    re[3] = 999;
    im[3] = 999;
    FFTReal(4).inverse(re, im, back);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(nyquist)
{
    double in[] = { 1, -1, 1, -1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_EQUAL(re[1], 0.0);
    BOOST_CHECK_EQUAL(re[2], 4.0);
    BOOST_CHECK_EQUAL(re[3], 0.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
    COMPARE_CONST(backim, 0.0);
}

BOOST_AUTO_TEST_CASE(r_nyquist)
{
    double in[] = { 1, -1, 1, -1 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFTReal(4).forward(in, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_EQUAL(re[1], 0.0);
    BOOST_CHECK_EQUAL(re[2], 4.0);
    BOOST_CHECK_EQUAL(re[3], 0.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    // check conjugates are reconstructed
    re[3] = 999;
    im[3] = 999;
    FFTReal(4).inverse(re, im, back);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(dirac)
{
    double in[] = { 1, 0, 0, 0 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 1.0);
    BOOST_CHECK_EQUAL(re[1], 1.0);
    BOOST_CHECK_EQUAL(re[2], 1.0);
    BOOST_CHECK_EQUAL(re[3], 1.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
    COMPARE_CONST(backim, 0.0);
}

BOOST_AUTO_TEST_CASE(r_dirac)
{
    double in[] = { 1, 0, 0, 0 };
    double re[] = { 999, 999, 999, 999 };
    double im[] = { 999, 999, 999, 999 };
    FFTReal(4).forward(in, re, im);
    BOOST_CHECK_EQUAL(re[0], 1.0);
    BOOST_CHECK_EQUAL(re[1], 1.0);
    BOOST_CHECK_EQUAL(re[2], 1.0);
    BOOST_CHECK_EQUAL(re[3], 1.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    // check conjugates are reconstructed
    re[3] = 999;
    im[3] = 999;
    FFTReal(4).inverse(re, im, back);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_SUITE_END()

