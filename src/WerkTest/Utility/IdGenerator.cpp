
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

#include <boost/test/unit_test.hpp>

#include "Werk/Utility/IdGenerator.hpp"

BOOST_AUTO_TEST_SUITE(IdGeneratorTest)

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::IdGenerator<> g;
	BOOST_REQUIRE_EQUAL(g.nextId(), 0);
	BOOST_REQUIRE_EQUAL(g.nextId(), 0);
	BOOST_REQUIRE_EQUAL(g.getNext(), 0);
	BOOST_REQUIRE_EQUAL(g.nextId(), 1);
	BOOST_REQUIRE_EQUAL(g.getNext(), 1);
	BOOST_REQUIRE_EQUAL(g.nextId(), 2);
	BOOST_REQUIRE_EQUAL(g.getNext(), 2);
	BOOST_REQUIRE_EQUAL(g.nextId(), 3);
	BOOST_REQUIRE_EQUAL(g.getNext(), 3);
	BOOST_REQUIRE_EQUAL(g.getNext(), 4);
	BOOST_REQUIRE_EQUAL(g.getNext(), 5);
}

BOOST_AUTO_TEST_SUITE_END()
