
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <cmath>

#include <boost/test/unit_test.hpp>

#include "Werk/Math/DiscreteEma.hpp"

BOOST_AUTO_TEST_SUITE(DiscreteEmaTest)

BOOST_AUTO_TEST_CASE(testEmpty)
{
	werk::DiscreteEma ema(0.5);

	BOOST_CHECK_EQUAL(ema.alpha(), 0.5);
	BOOST_CHECK(std::isnan(ema.value()));
}

BOOST_AUTO_TEST_CASE(testConstant)
{
	werk::DiscreteEma ema(0.5);

	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
}

BOOST_AUTO_TEST_CASE(testDecreasing)
{
	werk::DiscreteEma ema(0.5);

	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);

	ema.sample(0.0);
	BOOST_CHECK(ema.value() < 10.0);

	double value = ema.value();
	ema.sample(0.0);
	BOOST_CHECK(ema.value() < value);
}

BOOST_AUTO_TEST_CASE(testIncreasing)
{
	werk::DiscreteEma ema(0.5);

	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(13.0);
	BOOST_CHECK(ema.value() > 10);

	double value = ema.value();
	ema.sample(20.0);
	BOOST_CHECK(ema.value() > value);
}

BOOST_AUTO_TEST_CASE(testOscillating)
{
	werk::DiscreteEma ema(0.5);

	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(0.0);
	BOOST_CHECK_CLOSE(ema.value(), 5.0, 0.5);
	ema.sample(10.0);
	BOOST_CHECK_CLOSE(ema.value(), 7.5, 0.5);
	ema.sample(0.0);
	BOOST_CHECK_CLOSE(ema.value(), 3.75, 0.5);
}

BOOST_AUTO_TEST_CASE(testNan)
{
	werk::DiscreteEma ema(0.5);

	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(std::numeric_limits<double>::quiet_NaN());
	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
}

BOOST_AUTO_TEST_CASE(testZeroAlpha)
{
	werk::DiscreteEma ema(0.0);

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(15.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(2310.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
}

BOOST_AUTO_TEST_SUITE_END()
