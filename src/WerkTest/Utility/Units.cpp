
#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Units.hpp"

BOOST_AUTO_TEST_SUITE(UnitsTest) //haha

BOOST_AUTO_TEST_CASE(TestStorage)
{
	BOOST_REQUIRE_EQUAL(werk::parseUnits("124", werk::STORAGE_UNITS), 124);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("723", werk::STORAGE_UNITS), 723);

	BOOST_REQUIRE_EQUAL(werk::parseUnits("1.5K", werk::STORAGE_UNITS), 1024 + 512);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("4M", werk::STORAGE_UNITS), 4 * 1024 * 1024);
}

BOOST_AUTO_TEST_CASE(TestTime)
{
	BOOST_REQUIRE_EQUAL(werk::parseUnits("124", werk::TIME_UNITS), 124);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("723", werk::TIME_UNITS), 723);

	BOOST_REQUIRE_EQUAL(werk::parseUnits("10us", werk::TIME_UNITS), 10000);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("123.456ms", werk::TIME_UNITS), 123456000);
}

BOOST_AUTO_TEST_SUITE_END()
