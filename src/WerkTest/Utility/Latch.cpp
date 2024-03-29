
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

#include "Werk/Utility/Latch.hpp"

BOOST_AUTO_TEST_SUITE(LatchTest)

BOOST_AUTO_TEST_CASE(TestBasics)
{
	werk::Latch<> l;
	BOOST_REQUIRE(!l.value());

	l.set();
	BOOST_REQUIRE(l.value());

	l.set();
	BOOST_REQUIRE(l.value());

	l.reset();
	BOOST_REQUIRE(!l.value());

	l.reset();
	BOOST_REQUIRE(!l.value());
}

BOOST_AUTO_TEST_CASE(TestSetResetLatchActions)
{
	werk::Latch<> l;
	werk::SetLatchAction<> s("SetLatch", l);
	werk::ResetAction<werk::Latch<> > r("ResetLatch", l);

	BOOST_REQUIRE(!l.value());

	s.execute();
	BOOST_REQUIRE(l.value());

	s.execute();
	BOOST_REQUIRE(l.value());

	r.execute();
	BOOST_REQUIRE(!l.value());

	r.execute();
	BOOST_REQUIRE(!l.value());
}

BOOST_AUTO_TEST_CASE(TestConditionalAction)
{
	werk::Latch<> l;
	werk::CounterAction<> counter("Counter");
	werk::ConditionalAction<> c("Conditional", l, &counter);

	BOOST_REQUIRE_EQUAL(counter.count(), 0);

	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 0);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 0);
	counter.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 1);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 1);

	l.set();
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 2);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 3);

	l.reset();
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 3);
	c.execute();
	BOOST_REQUIRE_EQUAL(counter.count(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
