
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

#include "Werk/Utility/SlidingWindow.hpp"

BOOST_AUTO_TEST_SUITE(SlidingWindowTest)

BOOST_AUTO_TEST_CASE(TestSlidingWindow)
{
	werk::SlidingWindow<uint64_t, uint64_t, 4> w(1000);
	BOOST_REQUIRE_EQUAL(w.count(), 0);
	BOOST_REQUIRE(w.isEmpty());
	BOOST_REQUIRE(!w.isFull());

	w.sample(100, 2);
	BOOST_REQUIRE_EQUAL(w.count(), 1);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());

	w.sample(300, 4);
	BOOST_REQUIRE_EQUAL(w.count(), 2);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());

	w.sample(500, 6);
	BOOST_REQUIRE_EQUAL(w.count(), 3);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());

	w.sample(700, 8);
	BOOST_REQUIRE_EQUAL(w.count(), 4);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(w.isFull());

	//Force slide
	w.sample(900, 10);
	BOOST_REQUIRE_EQUAL(w.count(), 4);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(w.isFull());

	//Manually slide
	w.slide(1501);
	BOOST_REQUIRE_EQUAL(w.count(), 2);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());
}

BOOST_AUTO_TEST_CASE(TestSumSlidingWindow)
{
	werk::SumSlidingWindow<uint64_t, uint64_t, 4> w(1000);
	BOOST_REQUIRE_EQUAL(w.count(), 0);
	BOOST_REQUIRE(w.isEmpty());
	BOOST_REQUIRE(!w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 0);

	w.sample(100, 2);
	BOOST_REQUIRE_EQUAL(w.count(), 1);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 2);

	w.sample(300, 4);
	BOOST_REQUIRE_EQUAL(w.count(), 2);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 6);

	w.sample(500, 6);
	BOOST_REQUIRE_EQUAL(w.count(), 3);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 12);

	w.sample(700, 8);
	BOOST_REQUIRE_EQUAL(w.count(), 4);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 20);

	//Force slide
	w.sample(900, 10);
	BOOST_REQUIRE_EQUAL(w.count(), 4);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 28);

	//Manually slide
	w.slide(1501);
	BOOST_REQUIRE_EQUAL(w.count(), 2);
	BOOST_REQUIRE(!w.isEmpty());
	BOOST_REQUIRE(!w.isFull());
	BOOST_REQUIRE_EQUAL(w.sum(), 18);
}

BOOST_AUTO_TEST_SUITE_END()
