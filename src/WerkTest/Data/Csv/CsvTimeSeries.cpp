
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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

BOOST_AUTO_TEST_SUITE(CsvTimeSeriesTest)

BOOST_AUTO_TEST_CASE(testBasic)
{
	werk::CsvParser csvParser;
	BOOST_REQUIRE(csvParser.open("src/WerkTest/Data/Csv/test.csv"));

	//Skip headers
	BOOST_REQUIRE(csvParser.moveNext());

	werk::CsvTimeSeries csvTimeSeries("Csv", csvParser, 0);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 10);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 20);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 25);

	BOOST_REQUIRE(csvTimeSeries.moveNext());
	BOOST_CHECK_EQUAL(csvTimeSeries.time(), 35);

	BOOST_REQUIRE(!csvParser.moveNext());
}

BOOST_AUTO_TEST_SUITE_END()
