
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
