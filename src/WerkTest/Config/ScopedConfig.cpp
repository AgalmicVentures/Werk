
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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
#include "Werk/Config/MapConfigSource.hpp"
#include "Werk/Config/ScopedConfig.hpp"
#include "Werk/Logging/Log.hpp"

BOOST_AUTO_TEST_SUITE(ScopedConfigTest)

BOOST_AUTO_TEST_CASE(TestBasicTypes)
{
	werk::NullLog log("null");
	werk::Config c("config", &log);

	werk::MapConfigSource mapConfigSource;

	mapConfigSource.values()["Pi"] = "3";
	mapConfigSource.values()["*.Pi"] = "3.125";
	mapConfigSource.values()["A.Pi"] = "3.25";

	mapConfigSource.values()["E"] = "2";
	mapConfigSource.values()["*.E"] = "2.5";
	mapConfigSource.values()["A.E"] = "2.75";

	c.addConfigSource(&mapConfigSource);
	c.reloadConfig();
	c.execute();

	werk::ScopedConfig sc(c);

	//With no scoped declared, values come from the unprefixed configs
	BOOST_REQUIRE_EQUAL(sc.getString("Pi"), "3");
	BOOST_REQUIRE_EQUAL(sc.getDouble("Pi"), 3);
	BOOST_REQUIRE_EQUAL(sc.getString("E"), "2");
	BOOST_REQUIRE_EQUAL(sc.getDouble("E"), 2);

	//With a bit of scope, now they come from that
	sc.scopes().push_back("*.");
	BOOST_REQUIRE_EQUAL(sc.getString("Pi"), "3.125");
	BOOST_REQUIRE_EQUAL(sc.getDouble("Pi"), 3.125);
	BOOST_REQUIRE_EQUAL(sc.getString("E"), "2.5");
	BOOST_REQUIRE_EQUAL(sc.getDouble("E"), 2.5);

	//With a bit more scope, now they come from the innermost scope
	sc.scopes().insert(sc.scopes().begin(), "A.");
	BOOST_REQUIRE_EQUAL(sc.getString("Pi"), "3.25");
	BOOST_REQUIRE_EQUAL(sc.getDouble("Pi"), 3.25);
	BOOST_REQUIRE_EQUAL(sc.getString("E"), "2.75");
	BOOST_REQUIRE_EQUAL(sc.getDouble("E"), 2.75);
}

BOOST_AUTO_TEST_SUITE_END()
