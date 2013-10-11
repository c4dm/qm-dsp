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

void
testResampler(int sourceRate,
	      int targetRate,
	      int n,
	      double *in,
	      int m,
	      double *expected)
{
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

    int got = r.process(inPadded, outPadded, n1);

    std::cerr << n1 << " in, " << got << " out" << std::endl;

    BOOST_CHECK_EQUAL(got, m1);

    std::cerr << "results including latency padding:" << std::endl;
    for (int i = 0; i < m1; ++i) {
	std::cerr << outPadded[i] << " ";
	if (i % 6 == 5) std::cerr << "\n";
    }
    std::cerr << "\n";

    for (int i = latency; i < m1; ++i) {
	BOOST_CHECK_CLOSE(outPadded[i], expected[i-latency], 1e-8);
    }
    delete[] outPadded;
    delete[] inPadded;
}

BOOST_AUTO_TEST_CASE(sameRate)
{
    double d[] = { 0, 0.1, -0.3, -0.4, -0.3, 0, 0.5, 0.2, 0.8, -0.1 };
    testResampler(4, 4, 10, d, 10, d);
}

BOOST_AUTO_TEST_SUITE_END()

