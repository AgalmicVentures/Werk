
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

BOOST_AUTO_TEST_SUITE(CsvParserTest)

BOOST_AUTO_TEST_CASE(testBasic)
{
	werk::CsvParser csvParser;
	BOOST_REQUIRE(csvParser.open("src/WerkTest/Data/Csv/test.csv"));

	BOOST_REQUIRE(csvParser.moveNext(true));
	BOOST_CHECK_EQUAL(csvParser.headers()[0], "Time");
	BOOST_CHECK_EQUAL(csvParser.headers()[1], "X");
	BOOST_CHECK_EQUAL(csvParser.headers()[2], "Y");
	BOOST_CHECK_EQUAL(csvParser.headers()[3], "Z");
	BOOST_CHECK_EQUAL(csvParser.headers()[4], "Comment");
	BOOST_CHECK_EQUAL(csvParser.values().size(), 0);
	BOOST_CHECK_EQUAL(csvParser.lastValues().size(), 0);

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "10");
	BOOST_CHECK_EQUAL(csvParser.values()[4], "this is a comment"); //Do spaces work?
	BOOST_CHECK_EQUAL(csvParser.values().size(), 5);
	BOOST_CHECK_EQUAL(csvParser.lastValues().size(), 0);

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "20");
	BOOST_CHECK_EQUAL(csvParser.values()[4], "a quoted comment"); //Do quotes work?
	BOOST_CHECK_EQUAL(csvParser.values().size(), 5);
	BOOST_CHECK_EQUAL(csvParser.lastValues().size(), 5);

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "25");
	BOOST_CHECK_EQUAL(csvParser.values()[4], "a quoted comment, with a comma"); //Do commas work?
	BOOST_CHECK_EQUAL(csvParser.values().size(), 5);
	BOOST_CHECK_EQUAL(csvParser.lastValues().size(), 5);

	BOOST_REQUIRE(csvParser.moveNext());
	BOOST_CHECK_EQUAL(csvParser.values()[0], "35");

	BOOST_REQUIRE(!csvParser.moveNext());
}

BOOST_AUTO_TEST_SUITE_END()
