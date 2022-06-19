
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <boost/test/unit_test.hpp>

#include "Werk/Math/SlidingSummaryStatistics.hpp"

BOOST_AUTO_TEST_SUITE(SlidingSummaryStatisticsTest)

BOOST_AUTO_TEST_CASE(TestInitialize)
{
	werk::SlidingSummaryStatistics<uint64_t, double, 1000> s(1'000'000'000);
    BOOST_CHECK_EQUAL(s.count(), 0);
    BOOST_CHECK_EQUAL(s.statistics().count(), 0);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 0);
}

BOOST_AUTO_TEST_CASE(TestSample)
{
    werk::SlidingSummaryStatistics<uint64_t, double, 1000> s(1'000'000'000);

    s.sample(1'000'000'000, 3.0);
    BOOST_CHECK_EQUAL(s.count(), 1);
    BOOST_CHECK_EQUAL(s.statistics().count(), 1);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 1.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 3.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 3.0);

    s.sample(1'500'000'000, 5.0);
    BOOST_CHECK_EQUAL(s.count(), 2);
    BOOST_CHECK_EQUAL(s.statistics().count(), 2);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 2.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 8.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 4.0);

    s.sample(2'000'000'000, 7.0);
    BOOST_CHECK_EQUAL(s.count(), 3);
    BOOST_CHECK_EQUAL(s.statistics().count(), 3);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 3.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 15.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 5.0);

    s.slide(2'000'000'000);
    BOOST_CHECK_EQUAL(s.count(), 3);
    BOOST_CHECK_EQUAL(s.statistics().count(), 3);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 3.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 15.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 5.0);

    s.slide(2'000'000'001);
    BOOST_CHECK_EQUAL(s.count(), 2);
    BOOST_CHECK_EQUAL(s.statistics().count(), 4);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 2.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 12.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 6.0);

    s.slide(2'500'000'001);
    BOOST_CHECK_EQUAL(s.count(), 1);
    BOOST_CHECK_EQUAL(s.statistics().count(), 5);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 1.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 7.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 7.0);

    s.slide(3'000'000'001);
    BOOST_CHECK_EQUAL(s.count(), 0);
    BOOST_CHECK_EQUAL(s.statistics().count(), 0);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 0.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 0.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 0.0);
}

BOOST_AUTO_TEST_CASE(TestReset)
{
    werk::SlidingSummaryStatistics<uint64_t, double, 1000> s(1'000'000'000);

    s.sample(1'000'000'000, 3.0);
    s.sample(1'500'000'000, 5.0);
    s.sample(2'000'000'000, 7.0);

    s.reset();
    BOOST_CHECK_EQUAL(s.count(), 0);
    BOOST_CHECK_EQUAL(s.statistics().count(), 0);
    BOOST_CHECK_EQUAL(s.statistics().weightSum(), 0.0);
    BOOST_CHECK_EQUAL(s.statistics().sum(), 0.0);
    BOOST_CHECK_EQUAL(s.statistics().average(), 0.0);
}

BOOST_AUTO_TEST_SUITE_END()
