
#include <cmath>

#include <boost/test/unit_test.hpp>

#include "Werk/Data/Csv/CsvTimeSeries.hpp"
#include "Werk/Simulation/Simulator.hpp"

BOOST_AUTO_TEST_SUITE(SimulatorTest)

BOOST_AUTO_TEST_CASE(testEmpty)
{
	werk::Clock clock;
	werk::Latch<volatile bool> complete;
	werk::Simulator simulator("Simulator", &clock, complete, &werk::NULL_ACTION);

	//With no data sources, it will be immediately complete
	BOOST_CHECK(!complete.value());
	simulator.execute();
	BOOST_CHECK(complete.value());
}

BOOST_AUTO_TEST_CASE(testBasic)
{
	//Open and skip headers
	werk::CsvParser csvParser;
	werk::CsvTimeSeries csvTimeSeries(csvParser, 0, 1e9);
	BOOST_REQUIRE(csvParser.open("src/WerkTest/Data/Csv/test.csv"));
	BOOST_REQUIRE(csvParser.moveNext());

	werk::Clock clock;
	werk::Latch<volatile bool> complete;
	werk::Simulator simulator("Simulator", &clock, complete, &werk::NULL_ACTION);
	simulator.addDataSource(&csvTimeSeries);

	simulator.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 10e9);

	simulator.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 20e9);

	simulator.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 25e9);

	simulator.execute();
	BOOST_CHECK(complete.value());
}

BOOST_AUTO_TEST_SUITE_END()
