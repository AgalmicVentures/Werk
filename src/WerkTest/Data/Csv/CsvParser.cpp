
#include <cmath>

#include <boost/test/unit_test.hpp>

#include "Werk/Data/Csv/CsvTimeSeries.hpp"

BOOST_AUTO_TEST_SUITE(CsvParserTest)

BOOST_AUTO_TEST_CASE(testBasic)
{
	werk::CsvParser csvParser;
	BOOST_REQUIRE(csvParser.open("src/WerkTest/Data/Csv/test.csv"));

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "Time");
	BOOST_CHECK_EQUAL(csvParser.values()[1], "X");
	BOOST_CHECK_EQUAL(csvParser.values()[2], "Y");
	BOOST_CHECK_EQUAL(csvParser.values()[3], "Z");

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "10");

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "20");

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "25");

	BOOST_REQUIRE(!csvParser.moveNext());
}

BOOST_AUTO_TEST_SUITE_END()
