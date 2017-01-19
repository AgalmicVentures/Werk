
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

BOOST_AUTO_TEST_SUITE_END()
