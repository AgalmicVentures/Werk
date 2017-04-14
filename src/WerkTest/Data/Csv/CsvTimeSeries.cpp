
#include <cmath>

#include <boost/test/unit_test.hpp>

#include "Werk/Data/Csv/CsvTimeSeries.hpp"

BOOST_AUTO_TEST_SUITE(CsvTimeSeriesTest)

BOOST_AUTO_TEST_CASE(testBasic)
{
	werk::CsvParser csvParser;
	BOOST_REQUIRE(csvParser.open("src/WerkTest/Data/Csv/test.csv"));

	//Skip headers
	BOOST_REQUIRE(csvParser.moveNext());

	werk::CsvTimeSeries csvTimeSeries(csvParser, 0);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 10);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 20);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 25);

	BOOST_REQUIRE(!csvParser.moveNext());
}

BOOST_AUTO_TEST_SUITE_END()
