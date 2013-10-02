/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "dsp/phasevocoder/PhaseVocoder.h"

#include "base/Window.h"

#include <iostream>

using std::cerr;
using std::endl;

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestFFT)

#define COMPARE_CONST(a, n) \
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_SMALL(a[cmp_i] - n, 1e-7);				\
    }

#define COMPARE_ARRAY(a, b)						\
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_SMALL(a[cmp_i] - b[cmp_i], 1e-7);			\
    }

#define COMPARE_ARRAY_EXACT(a, b)						\
    for (int cmp_i = 0; cmp_i < (int)(sizeof(a)/sizeof(a[0])); ++cmp_i) { \
        BOOST_CHECK_EQUAL(a[cmp_i], b[cmp_i]);			\
    }

BOOST_AUTO_TEST_CASE(fullcycle)
{
    // Cosine with one cycle exactly equal to pvoc hopsize. This is
    // pretty much the most trivial case -- in fact it's
    // indistinguishable from totally silent input (in the phase
    // values) because the measured phases are zero throughout.

    // We aren't windowing the input frame because (for once) it
    // actually *is* just a short part of a continuous infinite
    // sinusoid.

    double frame[] = { 1, 0, -1, 0, 1, 0, -1, 0 };

    PhaseVocoder pvoc(8, 4);

    // Make these arrays one element too long at each end, so as to
    // test for overruns. For frame size 8, we expect 8/2+1 = 5
    // mag/phase pairs.
    double mag[]   = { 999, 999, 999, 999, 999, 999, 999 };
    double phase[] = { 999, 999, 999, 999, 999, 999, 999 };
    double unw[]   = { 999, 999, 999, 999, 999, 999, 999 };

    pvoc.process(frame, mag + 1, phase + 1, unw + 1);

    double magExpected0[] = { 999, 0, 0, 4, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(mag, magExpected0);

    double phaseExpected0[] = { 999, 0, 0, 0, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(phase, phaseExpected0);

    double unwExpected0[] = { 999, 0, 0, 0, 0, 0, 999 };
    COMPARE_ARRAY(unw, unwExpected0);

    pvoc.process(frame, mag + 1, phase + 1, unw + 1);

    double magExpected1[] = { 999, 0, 0, 4, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(mag, magExpected1);

    double phaseExpected1[] = { 999, 0, 0, 0, 0, 0, 999 };
    COMPARE_ARRAY(phase, phaseExpected1);

    // Derivation of unwrapped values:
    // 
    // * Bin 0 (DC) always has phase 0 and expected phase 0
    //
    // * Bin 1 has expected phase pi (the hop size is half a cycle at
    //   its frequency), but measured phase 0 (because there is no
    //   signal in that bin). So it has phase error -pi, which is
    //   mapped into (-pi,pi] range as pi, giving an unwrapped phase
    //   of 2*pi.
    //  
    // * Bin 2 has expected unwrapped phase 2*pi, measured phase 0,
    //   hence error 0 and unwrapped phase 2*pi.
    //
    // * Bin 3 is like bin 1: it has expected phase 3*pi, measured
    //   phase 0, so phase error -pi and unwrapped phase 4*pi.
    //
    // * Bin 4 (Nyquist) is like bin 2: expected phase 4*pi, measured
    //   phase 0, hence error 0 and unwrapped phase 4*pi.

    double unwExpected1[] = { 999, 0, 2*M_PI, 2*M_PI, 4*M_PI, 4*M_PI, 999 };
    COMPARE_ARRAY(unw, unwExpected1);

    pvoc.process(frame, mag + 1, phase + 1, unw + 1);

    double magExpected2[] = { 999, 0, 0, 4, 0, 0, 999 };
    COMPARE_ARRAY_EXACT(mag, magExpected2);

    double phaseExpected2[] = { 999, 0, 0, 0, 0, 0, 999 };
    COMPARE_ARRAY(phase, phaseExpected2);

    double unwExpected2[] = { 999, 0, 4*M_PI, 4*M_PI, 8*M_PI, 8*M_PI, 999 };
    COMPARE_ARRAY(unw, unwExpected2);
}

BOOST_AUTO_TEST_CASE(overlapping)
{
    // Sine (i.e. cosine starting at phase -pi/2) starting with the
    // first sample, introducing a cosine of half the frequency
    // starting at the fourth sample, i.e. the second hop. The cosine
    // is introduced "by magic", i.e. it doesn't appear in the second
    // half of the first frame (it would have quite strange effects on
    // the first frame if it did).

    double data[32] = { // 3 x 8-sample frames which we pretend are overlapping
	0, 1, 0, -1, 0, 1, 0, -1,
	1, 1.70710678, 0, -1.70710678, -1, 0.29289322, 0, -0.29289322,
	-1, 0.29289322, 0, -0.29289322, 1, 1.70710678, 0, -1.70710678,
    };

    PhaseVocoder pvoc(8, 4);

    // Make these arrays one element too long at each end, so as to
    // test for overruns. For frame size 8, we expect 8/2+1 = 5
    // mag/phase pairs.
    double mag[]   = { 999, 999, 999, 999, 999, 999, 999 };
    double phase[] = { 999, 999, 999, 999, 999, 999, 999 };
    double unw[]   = { 999, 999, 999, 999, 999, 999, 999 };

cerr << "process 0" << endl;

    pvoc.process(data, mag + 1, phase + 1, unw + 1);

    double magExpected0[] = { 999, 0, 0, 4, 0, 0, 999 };
    COMPARE_ARRAY(mag, magExpected0);

    double phaseExpected0[] = { 999, 0, 0, -M_PI/2 , 0, 0, 999 };
    COMPARE_ARRAY(phase, phaseExpected0);

    double unwExpected0[] = { 999, 0, 0, -M_PI/2, 0, 0, 999 };
    COMPARE_ARRAY(unw, unwExpected0);

cerr << "process 1" << endl;

    pvoc.process(data + 8, mag + 1, phase + 1, unw + 1);

    double magExpected1[] = { 999, 0, 4, 4, 0, 0, 999 };
    COMPARE_ARRAY(mag, magExpected1);

    //!!! I don't know why [2] here is -M_PI and not M_PI; and I definitely don't know why [4] here is M_PI. Check these with care
    double phaseExpected1[] = { 999, 0, -M_PI, -M_PI/2, M_PI, 0, 999 };
    COMPARE_ARRAY(phase, phaseExpected1);

    double unwExpected1[] = { 999, 0, M_PI, 3*M_PI/2, 3*M_PI, 4*M_PI, 999 };
    COMPARE_ARRAY(unw, unwExpected1);

cerr << "process 2" << endl;

    pvoc.process(data + 16, mag + 1, phase + 1, unw + 1);

    double magExpected2[] = { 999, 0, 4, 4, 0, 0, 999 };
    COMPARE_ARRAY(mag, magExpected2);

    double phaseExpected2[] = { 999, 0, 0, -M_PI/2, 0, 0, 999 };
    COMPARE_ARRAY(phase, phaseExpected2);

    double unwExpected2[] = { 999, 0, 2*M_PI, 7*M_PI/2, 6*M_PI, 8*M_PI, 999 };
    COMPARE_ARRAY(unw, unwExpected2);
}

BOOST_AUTO_TEST_SUITE_END()

