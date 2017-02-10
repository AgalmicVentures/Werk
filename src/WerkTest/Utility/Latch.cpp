
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
