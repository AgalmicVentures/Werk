
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
