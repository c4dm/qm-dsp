/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

#include "maths/MathUtilities.h"

#include <cmath>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(TestMathUtilities)

BOOST_AUTO_TEST_CASE(round)
{
    BOOST_CHECK_EQUAL(MathUtilities::round(0.5), 1.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(0.49), 0.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(0.99), 1.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(0.01), 0.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(0.0), 0.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(100.0), 100.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(-0.2), 0.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(-0.5), -1.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(-0.99), -1.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(-1.0), -1.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(-1.1), -1.0);
    BOOST_CHECK_EQUAL(MathUtilities::round(-1.5), -2.0);
}

BOOST_AUTO_TEST_CASE(mean)
{
    BOOST_CHECK_EQUAL(MathUtilities::mean(0, 0), 0);
    double d0[] = { 0, 4, 3, -1 };
    BOOST_CHECK_EQUAL(MathUtilities::mean(d0, 4), 1.5);
    double d1[] = { -2.6 };
    BOOST_CHECK_EQUAL(MathUtilities::mean(d1, 1), -2.6);
    std::vector<double> v;
    v.push_back(0);
    v.push_back(4);
    v.push_back(3);
    v.push_back(-1);
    BOOST_CHECK_EQUAL(MathUtilities::mean(v, 0, 4), 1.5);
    BOOST_CHECK_EQUAL(MathUtilities::mean(v, 1, 2), 3.5);
    BOOST_CHECK_EQUAL(MathUtilities::mean(v, 3, 1), -1);
    BOOST_CHECK_EQUAL(MathUtilities::mean(v, 3, 0), 0);
}

BOOST_AUTO_TEST_CASE(sum)
{
    BOOST_CHECK_EQUAL(MathUtilities::sum(0, 0), 0);
    double d0[] = { 0, 4, 3, -1 };
    BOOST_CHECK_EQUAL(MathUtilities::sum(d0, 4), 6);
    double d1[] = { -2.6 };
    BOOST_CHECK_EQUAL(MathUtilities::sum(d1, 1), -2.6);
}

BOOST_AUTO_TEST_CASE(median)
{
    BOOST_CHECK_EQUAL(MathUtilities::median(0, 0), 0);
    double d0[] = { 0, 4, 3, -1 };
    BOOST_CHECK_EQUAL(MathUtilities::median(d0, 4), 1.5);
    double d1[] = { 0, 4, 3, -1, -1 };
    BOOST_CHECK_EQUAL(MathUtilities::median(d1, 5), 0);
    double d2[] = { 1.0, -2.0 };
    BOOST_CHECK_EQUAL(MathUtilities::median(d2, 2), -0.5);
    double d3[] = { -2.6 };
    BOOST_CHECK_EQUAL(MathUtilities::median(d3, 1), -2.6);
}

BOOST_AUTO_TEST_CASE(princarg)
{
    BOOST_CHECK_EQUAL(MathUtilities::princarg(M_PI), M_PI);
    BOOST_CHECK_EQUAL(MathUtilities::princarg(-M_PI), M_PI);
    BOOST_CHECK_EQUAL(MathUtilities::princarg(2 * M_PI), 0.0);
    BOOST_CHECK_EQUAL(MathUtilities::princarg(5 * M_PI), M_PI);
    BOOST_CHECK_EQUAL(MathUtilities::princarg(1.0), 1.0);
    BOOST_CHECK_EQUAL(MathUtilities::princarg(-1.0), -1.0);
    BOOST_CHECK_EQUAL(MathUtilities::princarg(-10.0), -10.0 + 4 * M_PI);
}

BOOST_AUTO_TEST_CASE(isPowerOfTwo)
{
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(0), false);
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(1), true);
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(-2), false);
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(2), true);
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(3), false);
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(12), false);
    BOOST_CHECK_EQUAL(MathUtilities::isPowerOfTwo(16), true);
}

BOOST_AUTO_TEST_CASE(nextPowerOfTwo)
{
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(0), 1);
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(1), 1);
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(-2), 1);
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(2), 2);
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(3), 4);
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(12), 16);
    BOOST_CHECK_EQUAL(MathUtilities::nextPowerOfTwo(16), 16);
}

BOOST_AUTO_TEST_CASE(previousPowerOfTwo)
{
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(0), 1);
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(1), 1);
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(-2), 1);
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(2), 2);
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(3), 2);
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(12), 8);
    BOOST_CHECK_EQUAL(MathUtilities::previousPowerOfTwo(16), 16);
}

BOOST_AUTO_TEST_CASE(nearestPowerOfTwo)
{
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(0), 1);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(1), 1);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(-2), 1);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(2), 2);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(3), 4);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(11), 8);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(12), 16);
    BOOST_CHECK_EQUAL(MathUtilities::nearestPowerOfTwo(16), 16);
}

BOOST_AUTO_TEST_CASE(factorial)
{
    BOOST_CHECK_EQUAL(MathUtilities::factorial(-10), 0);
    BOOST_CHECK_EQUAL(MathUtilities::factorial(0), 1);
    BOOST_CHECK_EQUAL(MathUtilities::factorial(1), 1);
    BOOST_CHECK_EQUAL(MathUtilities::factorial(2), 2);
    BOOST_CHECK_EQUAL(MathUtilities::factorial(3), 6);
    BOOST_CHECK_EQUAL(MathUtilities::factorial(4), 24);
}

BOOST_AUTO_TEST_CASE(gcd)
{
    BOOST_CHECK_EQUAL(MathUtilities::gcd(1, 1), 1);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(2, 1), 1);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(2, 3), 1);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(4, 2), 2);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(18, 24), 6);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(27, 18), 9);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(18, 36), 18);
    BOOST_CHECK_EQUAL(MathUtilities::gcd(37, 18), 1);
}

BOOST_AUTO_TEST_SUITE_END()


