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
		     double *expected)
{
    vector<double> resampled = Resampler::resample(sourceRate, targetRate,
						   in, n);
    BOOST_CHECK_EQUAL(resampled.size(), m);
    for (int i = 0; i < m; ++i) {
	BOOST_CHECK_SMALL(resampled[i] - expected[i], 1e-8);
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
    std::cerr << "latency = " << latency << std::endl;

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
	std::cerr << "i = " << i << ", n1 = " << n1 << ", chunkSize = " << chunkSize << std::endl;
	got += r.process(inPadded + i, outPadded + got, chunkSize);
	i = i + chunkSize;
	chunkSize = chunkSize + 1;
	if (i + 1 >= n1) {
	    break;
	} else if (i + chunkSize >= n1) {
	    chunkSize = n1 - i;
	}
    }

//    int got = r.process(inPadded, outPadded, n1);
    std::cerr << i << " in, " << got << " out" << std::endl;

    BOOST_CHECK_EQUAL(got, m1);
/*
    std::cerr << "results including latency padding:" << std::endl;
    for (int i = 0; i < m1; ++i) {
	std::cerr << outPadded[i] << " ";
	if (i % 6 == 5) std::cerr << "\n";
    }
    std::cerr << "\n";
*/
    for (int i = latency; i < m1; ++i) {
	BOOST_CHECK_SMALL(outPadded[i] - expected[i-latency], 1e-8);
    }
    delete[] outPadded;
    delete[] inPadded;
}

BOOST_AUTO_TEST_CASE(sameRate)
{
    double d[] = { 0, 0.1, -0.3, -0.4, -0.3, 0, 0.5, 0.2, 0.8, -0.1 };
    testResamplerOneShot(4, 4, 10, d, 10, d);
    testResampler(4, 4, 10, d, 10, d);
}

BOOST_AUTO_TEST_SUITE_END()

