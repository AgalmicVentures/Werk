
/*
 * Copyright (c) 2015-2019 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Units.hpp"

BOOST_AUTO_TEST_SUITE(UnitsTest) //haha

BOOST_AUTO_TEST_CASE(TestFormatStorage)
{
	double scaledValue;
	const char *unitName;
	werk::formatUnits(124, werk::STORAGE_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 124);
	BOOST_REQUIRE_EQUAL(unitName, "");

	werk::formatUnits(723, werk::STORAGE_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 723);
	BOOST_REQUIRE_EQUAL(unitName, "");

	werk::formatUnits(1024 + 512, werk::STORAGE_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 1.5);
	BOOST_REQUIRE_EQUAL(unitName, "K");

	werk::formatUnits(4 * 1024 * 1024, werk::STORAGE_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 4.0);
	BOOST_REQUIRE_EQUAL(unitName, "M");
}

BOOST_AUTO_TEST_CASE(TestFormatTime)
{
	double scaledValue;
	const char *unitName;
	werk::formatUnits(124, werk::TIME_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 124);
	BOOST_REQUIRE_EQUAL(unitName, "ns");

	werk::formatUnits(723, werk::TIME_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 723);
	BOOST_REQUIRE_EQUAL(unitName, "ns");

	werk::formatUnits(10000, werk::TIME_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_EQUAL(scaledValue, 10.0);
	BOOST_REQUIRE_EQUAL(unitName, "us");

	werk::formatUnits(123456000, werk::TIME_UNITS, scaledValue, unitName);
	BOOST_REQUIRE_CLOSE(scaledValue, 123.456, 0.0000001);
	BOOST_REQUIRE_EQUAL(unitName, "ms");
}

BOOST_AUTO_TEST_CASE(TestParseStorage)
{
	BOOST_REQUIRE_EQUAL(werk::parseUnits("124", werk::STORAGE_UNITS), 124);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("723", werk::STORAGE_UNITS), 723);

	BOOST_REQUIRE_EQUAL(werk::parseUnits("1.5K", werk::STORAGE_UNITS), 1024 + 512);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("4M", werk::STORAGE_UNITS), 4 * 1024 * 1024);
}

BOOST_AUTO_TEST_CASE(TestParseTime)
{
	BOOST_REQUIRE_EQUAL(werk::parseUnits("124", werk::TIME_UNITS), 124);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("723", werk::TIME_UNITS), 723);

	BOOST_REQUIRE_EQUAL(werk::parseUnits("10us", werk::TIME_UNITS), 10000);
	BOOST_REQUIRE_EQUAL(werk::parseUnits("123.456ms", werk::TIME_UNITS), 123456000);
}

BOOST_AUTO_TEST_SUITE_END()
