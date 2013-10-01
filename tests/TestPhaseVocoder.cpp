
#include "dsp/phasevocoder/PhaseVocoder.h"

#include "base/Window.h"

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

#define COMPARE_ARRAY_EXACT(a, b)						\
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_EQUAL(a[cmp_i], b[cmp_i]);			\
    }

BOOST_AUTO_TEST_CASE(fullcycle)
{
    // Cosine with one cycle exactly equal to pvoc hopsize. We aren't
    // windowing the input frame because (for once) it actually *is*
    // just a short part of a continuous infinite sinusoid.

    double frame[] = { 1, 0, -1, 0, 1, 0, -1, 0 };

    PhaseVocoder pvoc(8);

    // Make these arrays one element too long at each end, so as to
    // test for overruns. For frame size 8, we expect 8/2+1 = 5
    // mag/phase pairs.
    double mag[]   = { 999, 999, 999, 999, 999, 999, 999 };
    double phase[] = { 999, 999, 999, 999, 999, 999, 999 };

    pvoc.process(frame, mag + 1, phase + 1);

    double magExpected0[] = { 999, 0, 0, 4, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(mag, magExpected0);

    double phaseExpected0[] = { 999, 0, 0, 0, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(phase, phaseExpected0);

    pvoc.process(frame, mag + 1, phase + 1);

    double magExpected1[] = { 999, 0, 0, 4, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(mag, magExpected1);

    double phaseExpected1[] = { 999, 0, 0, 2 * M_PI, 0, 0, 999 };
    COMPARE_ARRAY(phase, phaseExpected1);
}

BOOST_AUTO_TEST_SUITE_END()

