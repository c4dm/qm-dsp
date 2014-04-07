/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "maths/MedianFilter.h"

#include <cmath>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestMedianFilter)

BOOST_AUTO_TEST_CASE(odd)
{
    // A median filter of size N always retains a pool of N elements,
    // which start out all-zero. So the output median will remain zero
    // until N/2 elements have been pushed.
    MedianFilter<double> f(3);
    f.push(1);			// 0 0 1
    BOOST_CHECK_EQUAL(f.get(), 0);
    BOOST_CHECK_EQUAL(f.get(), 0);
    f.push(-3);			// 0 1 -3
    BOOST_CHECK_EQUAL(f.get(), 0);
    f.push(5);			// 1 -3 5
    BOOST_CHECK_EQUAL(f.get(), 1);
    f.push(7);			// -3 5 7
    BOOST_CHECK_EQUAL(f.get(), 5);
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(3);			// 5 7 3
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(3);			// 7 3 3
    BOOST_CHECK_EQUAL(f.get(), 3);
}

BOOST_AUTO_TEST_CASE(even)
{
    // Our median does not halve the difference (should it??), it just
    // returns the next element from the input set
    MedianFilter<double> f(4);
    f.push(1);			// 0 0 0 1
    BOOST_CHECK_EQUAL(f.get(), 0);
    BOOST_CHECK_EQUAL(f.get(), 0);
    f.push(-3);			// 0 0 1 -3
    BOOST_CHECK_EQUAL(f.get(), 0);
    f.push(5);			// 0 1 -3 5
    BOOST_CHECK_EQUAL(f.get(), 1);
    f.push(7);			// 1 -3 5 7
    BOOST_CHECK_EQUAL(f.get(), 5);
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(3);			// -3 5 7 3
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(3);			// 5 7 3 3
    BOOST_CHECK_EQUAL(f.get(), 5);
}

BOOST_AUTO_TEST_CASE(odd75)
{
    MedianFilter<double> f(5, 75.f);
    f.push(1);			// 0 0 0 0 1
    BOOST_CHECK_EQUAL(f.get(), 0);
    BOOST_CHECK_EQUAL(f.get(), 0);
    f.push(-3);			// 0 0 0 1 -3
    BOOST_CHECK_EQUAL(f.get(), 0);
    f.push(5);			// 0 0 1 -3 5
    BOOST_CHECK_EQUAL(f.get(), 1);
    f.push(7);			// 0 1 -3 5 7
    BOOST_CHECK_EQUAL(f.get(), 5);
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(3);			// 0 -3 5 7 3
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(3);			// 0 5 7 3 3
    BOOST_CHECK_EQUAL(f.get(), 5);
}

BOOST_AUTO_TEST_CASE(even75)
{
    MedianFilter<double> f(4, 75.f);
    f.push(1);			// 0 0 0 1
    BOOST_CHECK_EQUAL(f.get(), 1);
    BOOST_CHECK_EQUAL(f.get(), 1);
    f.push(-3);			// 0 0 1 -3
    BOOST_CHECK_EQUAL(f.get(), 1);
    f.push(5);			// 0 1 -3 5
    BOOST_CHECK_EQUAL(f.get(), 5);
    f.push(7);			// 1 -3 5 7
    BOOST_CHECK_EQUAL(f.get(), 7);
    BOOST_CHECK_EQUAL(f.get(), 7);
    f.push(3);			// -3 5 7 3
    BOOST_CHECK_EQUAL(f.get(), 7);
    f.push(3);			// 5 7 3 3
    BOOST_CHECK_EQUAL(f.get(), 7);
}

BOOST_AUTO_TEST_SUITE_END()

    
