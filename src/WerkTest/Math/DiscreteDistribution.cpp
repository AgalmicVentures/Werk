
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Math/DiscreteDistribution.hpp"

BOOST_AUTO_TEST_SUITE(DiscreteDistributionTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	werk::DiscreteDistribution<double> d;
	BOOST_REQUIRE_EQUAL(d.sampleCount(), 0);
	BOOST_REQUIRE_EQUAL(d.weightCount(), 0);
	BOOST_REQUIRE_EQUAL(d.weightSum(), 0.0);
}

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::DiscreteDistribution<double> d;
	d.sample(1.0, 1.0);
	d.sample(2.0, 0.5);
	d.sample(3.0, 1.0);
	BOOST_REQUIRE_EQUAL(d.sampleCount(), 3);
	BOOST_REQUIRE_EQUAL(d.weightCount(), 3);
	BOOST_REQUIRE_EQUAL(d.weightSum(), 2.5);

	d.sample(2.0, 1.5);
	BOOST_REQUIRE_EQUAL(d.sampleCount(), 4);
	BOOST_REQUIRE_EQUAL(d.weightCount(), 3);
	BOOST_REQUIRE_EQUAL(d.weightSum(), 4.0);

	BOOST_REQUIRE_EQUAL(d.pdf(1.0), 0.25);
	BOOST_REQUIRE_EQUAL(d.pdf(2.0), 0.5);
	BOOST_REQUIRE_EQUAL(d.pdf(3.0), 0.25);

	BOOST_REQUIRE_EQUAL(d.cdf(1.0), 0.25);
	BOOST_REQUIRE_EQUAL(d.cdf(2.0), 0.75);
	BOOST_REQUIRE_EQUAL(d.cdf(2.9), 0.75);
	BOOST_REQUIRE_EQUAL(d.cdf(3.0), 1.0);
	BOOST_REQUIRE_EQUAL(d.cdf(4.0), 1.0);
}

BOOST_AUTO_TEST_SUITE_END()
