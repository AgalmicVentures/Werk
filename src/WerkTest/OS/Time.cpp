
#include <boost/test/unit_test.hpp>

#include "Werk/OS/Time.hpp"

BOOST_AUTO_TEST_SUITE(ClockTest)

BOOST_AUTO_TEST_CASE(TestEpochNs)
{
    uint64_t t1 = werk::epochNs();
    BOOST_REQUIRE(t1 > 0);

    uint64_t t2 = werk::epochNs();
    BOOST_REQUIRE(t2 >= t1);

    uint64_t t3 = werk::epochNs();
    BOOST_REQUIRE(t3 >= t2);
}

BOOST_AUTO_TEST_CASE(TestClock)
{
    werk::Clock c;

    BOOST_REQUIRE_EQUAL(c.time(), 0);

    c.setTime(100);
    BOOST_REQUIRE_EQUAL(c.time(), 100);

    c.setRealTime();
    uint64_t t1 = c.time();

    c.setRealTime();
    uint64_t t2 = c.time();
    BOOST_REQUIRE(t2 >= t1);
}

BOOST_AUTO_TEST_SUITE_END()
