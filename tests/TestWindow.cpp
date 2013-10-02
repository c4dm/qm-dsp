/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "base/Window.h"

#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestWindow)

using std::cout;
using std::endl;

#define COMPARE_ARRAY(a, b)						\
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_SMALL(a[cmp_i] - b[cmp_i], 1e-4);			\
    }

void
testSymmetric(double *d, int n)
{
    for (int i = 0; i <= n/2; ++i) {
	BOOST_CHECK_CLOSE(d[i], d[n-i-1], 1e-10);
    }
}

BOOST_AUTO_TEST_CASE(periodic)
{
    // We can't actually test whether a function is periodic, given
    // only one cycle of it! But we can make sure that all but the
    // first sample is symmetric, which is what a symmetric window
    // becomes when generated in periodic mode
    double d[9];
    for (int n = 8; n <= 9; ++n) {
	for (int wt = (int)FirstWindow; wt <= (int)LastWindow; ++wt) {
	    for (int i = 0; i < n; ++i) d[i] = 1.0;
	    Window<double> w((WindowType)wt, n);
	    w.cut(d);
	    testSymmetric(d + 1, n - 1);
	}
    }
}

template <int N>
void testWindow(WindowType type, const double expected[N])
{
    double d[N];
    for (int i = 0; i < N; ++i) d[i] = 1.0;
    Window<double> w(type, N);
    w.cut(d);
    COMPARE_ARRAY(d, expected);

    double d0[N], d1[N];
    for (int i = 0; i < N; ++i) d0[i] = 0.5 + (1.0 / (N * 2)) * (i + 1);
    w.cut(d0, d1);
    for (int i = 0; i < N; ++i) {
	BOOST_CHECK_SMALL(d1[i] - d0[i] * expected[i], 1e-4);
    }
}

BOOST_AUTO_TEST_CASE(bartlett)
{
    double e1[] = { 1 };
    testWindow<1>(BartlettWindow, e1);

    double e2[] = { 0, 0 };
    testWindow<2>(BartlettWindow, e2);

    double e3[] = { 0, 2./3., 2./3. };
    testWindow<3>(BartlettWindow, e3);

    double e4[] = { 0, 1./2., 1., 1./2. };
    testWindow<4>(BartlettWindow, e4);

    double e5[] = { 0, 1./2., 1., 1., 1./2. };
    testWindow<5>(BartlettWindow, e5);

    double e6[] = { 0, 1./3., 2./3., 1., 2./3., 1./3. };
    testWindow<6>(BartlettWindow, e6);
}
    
BOOST_AUTO_TEST_CASE(hamming)
{
    double e1[] = { 1 };
    testWindow<1>(HammingWindow, e1);

    double e10[] = {
	0.0800, 0.1679, 0.3979, 0.6821, 0.9121,
        1.0000, 0.9121, 0.6821, 0.3979, 0.1679
    };
    testWindow<10>(HammingWindow, e10);
}
    
BOOST_AUTO_TEST_CASE(hann)
{
    double e1[] = { 1 };
    testWindow<1>(HanningWindow, e1);

    double e10[] = {
        0, 0.0955, 0.3455, 0.6545, 0.9045,
        1.0000, 0.9045, 0.6545, 0.3455, 0.0955,
    };
    testWindow<10>(HanningWindow, e10);
}
    
BOOST_AUTO_TEST_CASE(blackman)
{
    double e1[] = { 1 };
    testWindow<1>(BlackmanWindow, e1);

    double e10[] = {
        0, 0.0402, 0.2008, 0.5098, 0.8492,
        1.0000, 0.8492, 0.5098, 0.2008, 0.0402,
    };
    testWindow<10>(BlackmanWindow, e10);
}
    
BOOST_AUTO_TEST_SUITE_END()

