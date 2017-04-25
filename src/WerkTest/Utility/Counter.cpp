
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

#include "Werk/Utility/Counter.hpp"

BOOST_AUTO_TEST_SUITE(CounterTest)

BOOST_AUTO_TEST_CASE(TestAdd)
{
	werk::Counter<> c;
	BOOST_REQUIRE_EQUAL(c.value(), 0);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 1);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 2);

	c.reset();
	BOOST_REQUIRE_EQUAL(c.value(), 0);
}

BOOST_AUTO_TEST_CASE(TestMultiply)
{
	werk::Counter<uint64_t, 7, 3, werk::CounterMode::MULTIPLY> c;
	BOOST_REQUIRE_EQUAL(c.value(), 7);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 21);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 63);

	c.reset();
	BOOST_REQUIRE_EQUAL(c.value(), 7);
}

BOOST_AUTO_TEST_CASE(TestShift)
{
	werk::Counter<uint64_t, 1, 1, werk::CounterMode::LSHIFT> c;
	BOOST_REQUIRE_EQUAL(c.value(), 1);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 2);

	c.increment();
	BOOST_REQUIRE_EQUAL(c.value(), 4);

	c.reset();
	BOOST_REQUIRE_EQUAL(c.value(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
