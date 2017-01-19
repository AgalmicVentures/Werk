
#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Action.hpp"

BOOST_AUTO_TEST_SUITE(ActionTest)

BOOST_AUTO_TEST_CASE(TestCounter)
{
	werk::CounterAction<> a("Counter");
    BOOST_REQUIRE_EQUAL(a.count(), 0);

    a.execute();
    BOOST_REQUIRE_EQUAL(a.count(), 1);

    a.execute();
    BOOST_REQUIRE_EQUAL(a.count(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
