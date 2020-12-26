
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Data/Csv/CsvTimeSeries.hpp"
#include "Werk/Data/TimeSeriesReplayer.hpp"

BOOST_AUTO_TEST_SUITE(TimeSeriesReplayerTest)

BOOST_AUTO_TEST_CASE(testEmpty)
{
	werk::Clock clock;
	werk::Latch<std::atomic<bool> > complete;
	werk::TimeSeriesReplayer replayer("TimeSeriesReplayer", &clock, complete, &werk::NULL_ACTION);

	//With no data sources, it will be immediately complete
	BOOST_CHECK(!complete.value());
	replayer.execute();
	BOOST_CHECK(complete.value());
}

BOOST_AUTO_TEST_CASE(testBasic)
{
	//Open and skip headers
	werk::CsvParser csvParser;
	werk::CsvTimeSeries csvTimeSeries("Csv", csvParser, 0, 1e9);
	BOOST_REQUIRE(csvParser.open("src/WerkTest/Data/Csv/test.csv"));
	BOOST_REQUIRE(csvParser.moveNext());

	werk::Clock clock;
	werk::Latch<std::atomic<bool> > complete;
	werk::TimeSeriesReplayer replayer("TimeSeriesReplayer", &clock, complete, &werk::NULL_ACTION);
	replayer.addDataSource(&csvTimeSeries);

	replayer.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 10e9);

	replayer.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 20e9);

	replayer.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 25e9);

	replayer.execute();
	BOOST_CHECK(!complete.value());
	BOOST_CHECK_EQUAL(clock.time(), 35e9);

	replayer.execute();
	BOOST_CHECK(complete.value());
}

BOOST_AUTO_TEST_SUITE_END()
