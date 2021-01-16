
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

#include "Werk/OS/Time.hpp"

BOOST_AUTO_TEST_SUITE(ClockTest)

BOOST_AUTO_TEST_CASE(TestEpochTime)
{
	uint64_t t1 = werk::epochTime();
	BOOST_REQUIRE(t1 > 0);
}

BOOST_AUTO_TEST_CASE(TestMonotoneTime)
{
	uint64_t t1 = werk::monotoneTime();
	for (uint32_t i = 0; i < 100; ++i) {
		uint64_t t2 = werk::monotoneTime();
		BOOST_REQUIRE(t2 >= t1);
		t1 = t2;
	}
}

BOOST_AUTO_TEST_CASE(TestClock)
{
	werk::Clock c;

	BOOST_REQUIRE_EQUAL(c.time(), 0);

	c.setTime(100);
	BOOST_REQUIRE_EQUAL(c.time(), 100);

	c.setEpochTime();
	uint64_t t1 = c.time();

	c.setEpochTime();
	uint64_t t2 = c.time();
	BOOST_REQUIRE(t2 >= t1);
}

BOOST_AUTO_TEST_SUITE_END()
