
#include <boost/test/unit_test.hpp>

#include "Werk/Profiling/Profile.hpp"

BOOST_AUTO_TEST_SUITE(ProfileTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	werk::Profile p("Empty");
	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().count(), 0);
}

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::Profile p("Basic");
	p.start(1);
	p.stop(101);

	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().count(), 0);

	p.reset();
	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 0);

	uint64_t step = 100;
	for (uint64_t i = 1; i < 101; ++i) {
		p.start(step);
		p.stop(i * step + step);
	}

	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().average(), 5100);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().average(), 7600);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().average(), 9100);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().average(), 9600);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().average(), 10000);
}

BOOST_AUTO_TEST_SUITE_END()
