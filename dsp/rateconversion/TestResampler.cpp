/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "Resampler.h"

#include <iostream>

#include <cmath>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestResampler)

using std::cout;
using std::endl;
using std::vector;

void
testResamplerOneShot(int sourceRate,
		     int targetRate,
		     int n,
		     double *in,
		     int m,
		     double *expected,
		     int skip)
{
    vector<double> resampled = Resampler::resample(sourceRate, targetRate,
						   in, n);
    if (skip == 0) {
	BOOST_CHECK_EQUAL(resampled.size(), m);
    }
    for (int i = 0; i < m; ++i) {
	BOOST_CHECK_SMALL(resampled[i + skip] - expected[i], 1e-8);
    }
}

void
testResampler(int sourceRate,
	      int targetRate,
	      int n,
	      double *in,
	      int m,
	      double *expected)
{
    // Here we provide the input in chunks (of varying size)

    Resampler r(sourceRate, targetRate);
    int latency = r.getLatency();

    int m1 = m + latency;
    int n1 = int((m1 * sourceRate) / targetRate);

    double *inPadded = new double[n1];
    double *outPadded = new double[m1];

    for (int i = 0; i < n1; ++i) {
	if (i < n) inPadded[i] = in[i];
	else inPadded[i] = 0.0;
    }
    
    for (int i = 0; i < m1; ++i) {
	outPadded[i] = -999.0;
    }

    int chunkSize = 1;
    int got = 0;
    int i = 0;

    while (true) {
	got += r.process(inPadded + i, outPadded + got, chunkSize);
	i = i + chunkSize;
	chunkSize = chunkSize + 1;
	if (i >= n1) {
	    break;
	} else if (i + chunkSize >= n1) {
	    chunkSize = n1 - i;
	} else if (chunkSize > 15) {
	    chunkSize = 1;
	}
    }

    BOOST_CHECK_EQUAL(got, m1);

    for (int i = latency; i < m1; ++i) {
	BOOST_CHECK_SMALL(outPadded[i] - expected[i-latency], 1e-8);
    }

    delete[] outPadded;
    delete[] inPadded;
}

BOOST_AUTO_TEST_CASE(sameRateOneShot)
{
    double d[] = { 0, 0.1, -0.3, -0.4, -0.3, 0, 0.5, 0.2, 0.8, -0.1 };
    testResamplerOneShot(4, 4, 10, d, 10, d, 0);
}

BOOST_AUTO_TEST_CASE(sameRate)
{
    double d[] = { 0, 0.1, -0.3, -0.4, -0.3, 0, 0.5, 0.2, 0.8, -0.1 };
    testResampler(4, 4, 10, d, 10, d);
}

BOOST_AUTO_TEST_CASE(interpolatedMisc)
{
    // Interpolating any signal by N should give a signal in which
    // every Nth sample is the original signal
    double in[] = { 0, 0.1, -0.3, -0.4, -0.3, 0, 0.5, 0.2, 0.8, -0.1 };
    int n = sizeof(in)/sizeof(in[0]);
    for (int factor = 2; factor < 10; ++factor) {
	vector<double> out = Resampler::resample(6, 6 * factor, in, n);
	for (int i = 0; i < n; ++i) {
	    BOOST_CHECK_SMALL(out[i * factor] - in[i], 1e-5);
	}
    }
}

BOOST_AUTO_TEST_CASE(interpolatedSine)
{
    // Interpolating a sinusoid should give us a sinusoid, once we've
    // dropped the first few samples
    double in[1000];
    double out[2000];
    for (int i = 0; i < 1000; ++i) {
	in[i] = sin(i * M_PI / 2.0);
    }
    for (int i = 0; i < 2000; ++i) {
	out[i] = sin(i * M_PI / 4.0);
    }
    testResamplerOneShot(8, 16, 1000, in, 200, out, 512);
}

BOOST_AUTO_TEST_CASE(decimatedSine)
{
    // Decimating a sinusoid should give us a sinusoid, once we've
    // dropped the first few samples
    double in[2000];
    double out[1000];
    for (int i = 0; i < 2000; ++i) {
	in[i] = sin(i * M_PI / 8.0);
    }
    for (int i = 0; i < 1000; ++i) {
	out[i] = sin(i * M_PI / 4.0);
    }
    testResamplerOneShot(16, 8, 2000, in, 200, out, 256);
}

BOOST_AUTO_TEST_SUITE_END()

