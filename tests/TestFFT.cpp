
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

BOOST_AUTO_TEST_CASE(dc)
{
    // DC-only signal. The DC bin is purely real
    double in[] = { 1, 1, 1, 1 };
    double re[4], im[4];
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 4.0);
    BOOST_CHECK_EQUAL(re[1], 0.0);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(sine)
{
    // Sine. Output is purely imaginary
    double in[] = { 0, 1, 0, -1 };
    double re[4], im[4];
    FFT(4).process(false, in, 0, re, im);
    COMPARE_CONST(re, 0.0);
    BOOST_CHECK_EQUAL(im[0], 0.0);
    BOOST_CHECK_EQUAL(im[1], -2.0);
    BOOST_CHECK_EQUAL(im[2], 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(cosine)
{
    // Cosine. Output is purely real
    double in[] = { 1, 0, -1, 0 };
    double re[4], im[4];
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_EQUAL(re[1], 2.0);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
}
	
BOOST_AUTO_TEST_CASE(sineCosine)
{
    // Sine and cosine mixed
    double in[] = { 0.5, 1, -0.5, -1 };
    double re[4], im[4];
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_CLOSE(re[1], 1.0, 1e-12);
    BOOST_CHECK_EQUAL(re[2], 0.0);
    BOOST_CHECK_EQUAL(im[0], 0.0);
    BOOST_CHECK_CLOSE(im[1], -2.0, 1e-12);
    BOOST_CHECK_EQUAL(im[2], 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(nyquist)
{
    double in[] = { 1, -1, 1, -1 };
    double re[4], im[4];
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 0.0);
    BOOST_CHECK_EQUAL(re[1], 0.0);
    BOOST_CHECK_EQUAL(re[2], 4.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
}

BOOST_AUTO_TEST_CASE(dirac)
{
    double in[] = { 1, 0, 0, 0 };
    double re[4], im[4];
    FFT(4).process(false, in, 0, re, im);
    BOOST_CHECK_EQUAL(re[0], 1.0);
    BOOST_CHECK_EQUAL(re[1], 1.0);
    BOOST_CHECK_EQUAL(re[2], 1.0);
    COMPARE_CONST(im, 0.0);
    double back[4];
    double backim[4];
    FFT(4).process(true, re, im, back, backim);
    COMPARE_ARRAY(back, in);
}

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

BOOST_AUTO_TEST_CASE(inverseArrayBounds)
{
    // initialise bins to something recognisable, so we can tell
    // if they haven't been written
    double re[] = { 0, 1, 0 };
    double im[] = { 0, -2, 0 };
    double outre[] = { 999, 999, 999, 999, 999, 999 };
    double outim[] = { 999, 999, 999, 999, 999, 999 };
    FFT(4).process(false, re, im, outre+1, outim+1);
    // And check we haven't overrun the arrays
    BOOST_CHECK_EQUAL(outre[0], 999.0);
    BOOST_CHECK_EQUAL(outim[0], 999.0);
    BOOST_CHECK_EQUAL(outre[5], 999.0);
    BOOST_CHECK_EQUAL(outim[5], 999.0);
}

BOOST_AUTO_TEST_SUITE_END()

