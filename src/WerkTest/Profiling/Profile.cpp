
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Profiling/Profile.hpp"

BOOST_AUTO_TEST_SUITE(ProfileTest)

BOOST_AUTO_TEST_CASE(TestEmpty)
{
	werk::Profile p("Empty");
	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().count(), 0);
}

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::Profile p("Basic");
	p.start(1);
	p.stop(101);

	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().count(), 0);

	p.reset();
	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 0);

	uint64_t step = 100;
	for (uint64_t i = 1; i < 101; ++i) {
		p.start(step);
		p.stop(i * step + step);
	}

	BOOST_REQUIRE_EQUAL(p.orderStatistics().count(), 0);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f50Statistics().average(), 5100);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f75Statistics().average(), 7600);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f90Statistics().average(), 9100);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f95Statistics().average(), 9600);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().count(), 1);
	BOOST_REQUIRE_EQUAL(p.f99Statistics().average(), 10000);
}

BOOST_AUTO_TEST_SUITE_END()
