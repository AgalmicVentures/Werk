
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

#include <boost/test/unit_test.hpp>

#include "Werk/Config/Config.hpp"
#include "Werk/Config/IniConfigSource.hpp"
#include "Werk/Config/MapConfigSource.hpp"
#include "Werk/Logging/Log.hpp"

BOOST_AUTO_TEST_SUITE(IniConfigSourceTest)

BOOST_AUTO_TEST_CASE(TestMissingFile)
{
	werk::IniConfigSource iniConfigSource("azxcvsdf");

	std::map<std::string, std::string> values;
	BOOST_REQUIRE(!iniConfigSource.reloadConfig(values));
}

BOOST_AUTO_TEST_CASE(TestBasicFile)
{
	werk::IniConfigSource iniConfigSource("src/WerkTest/Config/TestConfig.ini");

	std::map<std::string, std::string> values;
	BOOST_REQUIRE(iniConfigSource.reloadConfig(values));

	BOOST_CHECK_EQUAL(values["abc"], "123");
	BOOST_CHECK_EQUAL(values["def"], " 456");
	BOOST_CHECK_EQUAL(values["zxcv"], "qwer");

	BOOST_CHECK_EQUAL(values["Section.SecVal1"], "QWERTY");
	BOOST_CHECK_EQUAL(values["Section.SecVal2"], "ASDF");
	BOOST_CHECK_EQUAL(values["Section2.Sec2Val1"], "ZXCV");
}

BOOST_AUTO_TEST_SUITE_END()
