
#include <boost/test/unit_test.hpp>

#include <time.h>

#include "Werk/Threading/BackgroundThread.hpp"
#include "Werk/Threading/Watchdog.hpp"

BOOST_AUTO_TEST_SUITE(WatchdogTest)

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::CounterAction<volatile uint64_t> counterAction("Counter");
	werk::BackgroundThread backgroundThread(nullptr, 1l * 1000 * 1000);

	werk::Watchdog watchdog("Watchdog", &backgroundThread.backgroundClock(), &counterAction, 20l * 1000 * 1000, 0);
	BOOST_REQUIRE_EQUAL(watchdog.interval(), 20l * 1000 * 1000);
	BOOST_REQUIRE_EQUAL(watchdog.allowedMisses(), 0);

	//Start running
	backgroundThread.addTask(&watchdog);

	//Sleep for a little more than half the frequency -- no action
	timespec delay { 0, 11l * 1000 * 1000 };
	nanosleep(&delay, nullptr);
	BOOST_REQUIRE(watchdog.latch());
	BOOST_REQUIRE_EQUAL(counterAction.count(), 0);

	//And again
	nanosleep(&delay, nullptr);
	BOOST_REQUIRE(watchdog.latch());
	BOOST_REQUIRE_EQUAL(counterAction.count(), 1);

	//And again
	nanosleep(&delay, nullptr);
	BOOST_REQUIRE(watchdog.latch());
	BOOST_REQUIRE_EQUAL(counterAction.count(), 1);

	//And again -- the action only executes the first time
	nanosleep(&delay, nullptr);
	BOOST_REQUIRE(watchdog.latch());
	BOOST_REQUIRE_EQUAL(counterAction.count(), 1);

	backgroundThread.stop();
}

BOOST_AUTO_TEST_SUITE_END()
