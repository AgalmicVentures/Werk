
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

#include "Werk/Math/SimpleLinearRegression.hpp"

BOOST_AUTO_TEST_SUITE(SimpleLinearRegressionTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	werk::SimpleLinearRegression r;
	BOOST_REQUIRE_EQUAL(r.count(), 0);
	BOOST_REQUIRE(std::isnan(r.beta()));
	BOOST_REQUIRE(std::isnan(r.correlation()));
	BOOST_REQUIRE(std::isnan(r.predict(2.0)));
}

BOOST_AUTO_TEST_CASE(TestLinear)
{
	werk::SimpleLinearRegression r;

	r.sample(1.0, 4.0);
	BOOST_REQUIRE_EQUAL(r.count(), 1);
	BOOST_REQUIRE(std::isnan(r.beta()));
	BOOST_REQUIRE(std::isnan(r.correlation()));
	BOOST_REQUIRE(std::isnan(r.predict(2.0)));

	r.sample(3.0, 8.0);
	BOOST_REQUIRE_EQUAL(r.count(), 2);
	BOOST_REQUIRE_EQUAL(r.beta(), 2.0);
	BOOST_REQUIRE_EQUAL(r.correlation(), 1.0);
	BOOST_REQUIRE_EQUAL(r.predict(2.0), 6.0);

	r.sample(5.0, 12.0);
	BOOST_REQUIRE_EQUAL(r.count(), 3);
	BOOST_REQUIRE_EQUAL(r.correlation(), 1.0);
	BOOST_REQUIRE_CLOSE(r.beta(), 2.0, 0.000000001);
	BOOST_REQUIRE_CLOSE(r.alpha(), 2.0, 0.000000001);
	BOOST_REQUIRE_CLOSE(r.predict(2.0), 6.0, 0.000000001);

	r.sample(6.0, 14.0);
	BOOST_REQUIRE_EQUAL(r.count(), 4);
	BOOST_REQUIRE_EQUAL(r.correlation(), 1.0);
	BOOST_REQUIRE_CLOSE(r.beta(), 2.0, 0.000000001);
	BOOST_REQUIRE_CLOSE(r.alpha(), 2.0, 0.000000001);
	BOOST_REQUIRE_CLOSE(r.predict(2.0), 6.0, 0.000000001);

	r.reset();
	BOOST_REQUIRE_EQUAL(r.count(), 0);
}

BOOST_AUTO_TEST_CASE(TestZeroCorrelation)
{
	werk::SimpleLinearRegression r;

	r.sample(-1.0, 3.0);
	r.sample(0.0, 0.0);
	r.sample(1.0, 3.0);
	BOOST_REQUIRE_EQUAL(r.count(), 3);
	BOOST_REQUIRE_EQUAL(r.correlation(), 0.0);
	BOOST_REQUIRE_EQUAL(r.beta(), 0.0);
	BOOST_REQUIRE_CLOSE(r.alpha(), 2.0, 0.000000001);
}

BOOST_AUTO_TEST_SUITE_END()
