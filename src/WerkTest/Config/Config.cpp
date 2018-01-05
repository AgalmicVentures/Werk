
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

BOOST_AUTO_TEST_SUITE(ConfigTest)

BOOST_AUTO_TEST_CASE(TestBasicTypes)
{
	werk::Clock clock;
	werk::SyncLog log("stdout", &clock);
	werk::Config c("config", &log);

	werk::MapConfigSource mapConfigSource;
	mapConfigSource.values()["Pi"] = "3.25";
	mapConfigSource.values()["Two"] = "2";
	mapConfigSource.values()["Nested.Value"] = "asdf";
	c.addConfigSource(&mapConfigSource);
	c.reloadConfig();
	c.execute();

	BOOST_REQUIRE_EQUAL(c.getString("Pi"), "3.25");
	BOOST_REQUIRE_EQUAL(c.getDouble("Pi"), 3.25);

	BOOST_REQUIRE_EQUAL(c.getString("Two"), "2");
	BOOST_REQUIRE_EQUAL(c.getInt64("Two", 0, "Help test"), 2);
	BOOST_REQUIRE_EQUAL(c.getUint64("Two"), 2);
}

BOOST_AUTO_TEST_CASE(TestDefaultValue)
{
	werk::NullLog log("null");
	werk::Config c("config", &log);

	BOOST_REQUIRE_EQUAL(c.getString("Pi", "3.25"), "3.25");
}

BOOST_AUTO_TEST_CASE(TestLoadFile)
{
	werk::NullLog log("null");
	werk::Config c("config", &log);

	werk::IniConfigSource iniConfigSource("src/WerkTest/Config/TestConfig.ini");
	c.addConfigSource(&iniConfigSource);
	c.reloadConfig();
	c.execute();

	BOOST_REQUIRE_EQUAL(c.getString("zxcv", "asdf"), "qwer");
	BOOST_REQUIRE_EQUAL(c.getInt64("abc"), 123);
}

BOOST_AUTO_TEST_SUITE_END()
