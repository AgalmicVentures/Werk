
#include <cmath>

#include <boost/test/unit_test.hpp>

#include "Werk/Math/ContinuousEma.hpp"

BOOST_AUTO_TEST_SUITE(ContinuousEmaTest)

BOOST_AUTO_TEST_CASE(testEmpty)
{
	werk::ContinuousEma ema(10.0);

	BOOST_CHECK_EQUAL(ema.halfLife(), 10.0);
	BOOST_CHECK(ema.factor() < 1.0 / 10.0);
	BOOST_CHECK(std::isnan(ema.value()));
}

BOOST_AUTO_TEST_CASE(testConstant)
{
	werk::ContinuousEma ema(10.0);

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(0.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(4.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(17.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
}

BOOST_AUTO_TEST_CASE(testDecreasing)
{
	werk::ContinuousEma ema(60.0);

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(0.0, 10.0);

	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(13.0, 0.0);
	BOOST_CHECK(ema.value() < 10.0);

	double value = ema.value();
	ema.sample(19.0, 0.0);
	BOOST_CHECK(ema.value() < value);
}

BOOST_AUTO_TEST_CASE(testIncreasing)
{
	werk::ContinuousEma ema(60.0);

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(0.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(7.0, 13.0);
	BOOST_CHECK(ema.value() > 10);

	double value = ema.value();
	ema.sample(19.0, 20.0);
	BOOST_CHECK(ema.value() > value);
}

BOOST_AUTO_TEST_CASE(testOscillating)
{
	werk::ContinuousEma ema(10.0);

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(0.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(10.0, 0.0);
	BOOST_CHECK_CLOSE(ema.value(), 5.0, 0.5);
	ema.sample(20.0, 10.0);
	BOOST_CHECK_CLOSE(ema.value(), 7.5, 0.5);
	ema.sample(30.0, 0.0);
	BOOST_CHECK_CLOSE(ema.value(), 3.75, 0.5);
}

BOOST_AUTO_TEST_CASE(testNan)
{
	werk::ContinuousEma ema(60.0);

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(0.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(3.0, std::numeric_limits<double>::quiet_NaN());
	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(5.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
}

BOOST_AUTO_TEST_CASE(testInfiniteHalfLife)
{
	werk::ContinuousEma ema(std::numeric_limits<double>::infinity());

	BOOST_CHECK(std::isnan(ema.value()));
	ema.sample(0.0, 10.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(3.0, 15.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
	ema.sample(15.0, 2310.0);
	BOOST_CHECK_EQUAL(ema.value(), 10.0);
}

BOOST_AUTO_TEST_SUITE_END()
