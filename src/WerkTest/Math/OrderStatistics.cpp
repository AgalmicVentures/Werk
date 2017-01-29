
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
