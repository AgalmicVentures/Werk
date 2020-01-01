
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Math/OrderStatistics.hpp"

BOOST_AUTO_TEST_SUITE(OrderStatisticsTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	werk::OrderStatistics<double> s;
	BOOST_REQUIRE_EQUAL(s.count(), 0);
}

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::OrderStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);
	s.sample(3.0);
	s.sample(4.0);
	s.sample(2.0);

	BOOST_REQUIRE_EQUAL(s.count(), 5);
	BOOST_REQUIRE_EQUAL(s.min(), 1.0);
	BOOST_REQUIRE_EQUAL(s.q1(), 2.0);
	BOOST_REQUIRE_EQUAL(s.median(), 3.0);
	BOOST_REQUIRE_EQUAL(s.q3(), 4.0);
	BOOST_REQUIRE_EQUAL(s.max(), 5.0);

	s.reset();
	BOOST_REQUIRE_EQUAL(s.count(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
