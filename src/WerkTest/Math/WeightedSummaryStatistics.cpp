
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/Math/WeightedSummaryStatistics.hpp"

BOOST_AUTO_TEST_SUITE(WeightedSummaryStatisticsTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	werk::WeightedSummaryStatistics s;
	BOOST_REQUIRE_EQUAL(s.count(), 0);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 0.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 0.0);
}

BOOST_AUTO_TEST_CASE(TestBasicUnweighted)
{
	werk::WeightedSummaryStatistics s;
	s.sample(5.0, 1.0);
	s.sample(1.0, 1.0);

	BOOST_REQUIRE_EQUAL(s.count(), 2);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 2.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 6.0);
	BOOST_REQUIRE_EQUAL(s.average(), 3.0);
	BOOST_REQUIRE_EQUAL(s.variance(), 4.0);
	BOOST_REQUIRE_EQUAL(s.stddev(), 2.0);

	s.reset();
	BOOST_REQUIRE_EQUAL(s.count(), 0);
}

BOOST_AUTO_TEST_CASE(TestBasicEqualWeights)
{
	werk::WeightedSummaryStatistics s;
	s.sample(5.0, 2.0);
	s.sample(1.0, 2.0);

	BOOST_REQUIRE_EQUAL(s.count(), 2);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 4.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 12.0);
	BOOST_REQUIRE_EQUAL(s.average(), 3.0);
	BOOST_REQUIRE_EQUAL(s.variance(), 4.0);
	BOOST_REQUIRE_EQUAL(s.stddev(), 2.0);
}

BOOST_AUTO_TEST_CASE(TestBasicUnequalWeights)
{
	werk::WeightedSummaryStatistics s;
	s.sample(5.0, 1.0);
	s.sample(1.0, 3.0);

	BOOST_REQUIRE_EQUAL(s.count(), 2);
	BOOST_REQUIRE_EQUAL(s.weightSum(), 4.0);
	BOOST_REQUIRE_EQUAL(s.sum(), 8.0);
	BOOST_REQUIRE_EQUAL(s.average(), 2.0);
	BOOST_REQUIRE_EQUAL(s.variance(), 3.0);
}

BOOST_AUTO_TEST_CASE(TestAdvancedEqualWeights)
{
	werk::SummaryStatistics<double> s1;
	werk::WeightedSummaryStatistics s2;

	for (double i = 0.0; i < 1000.0; ++i) {
		s1.sample(i);
		s2.sample(i, 1.0);

		BOOST_REQUIRE_EQUAL(s1.count(), s2.count());
		BOOST_REQUIRE_EQUAL(s1.sum(), s2.sum());
		BOOST_REQUIRE_EQUAL(s2.weightSum(), i + 1.0);
		BOOST_REQUIRE_EQUAL(s1.average(), s2.average());
		BOOST_REQUIRE_EQUAL(s1.variance(), s2.variance());
	}
}

BOOST_AUTO_TEST_SUITE_END()
