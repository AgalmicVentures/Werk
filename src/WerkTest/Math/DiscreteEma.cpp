
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
