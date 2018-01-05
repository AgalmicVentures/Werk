
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <time.h>

#include "Werk/Threading/BackgroundThread.hpp"
#include "Werk/Threading/Watchdog.hpp"

BOOST_AUTO_TEST_SUITE(WatchdogTest)

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::CounterAction<volatile uint64_t> counterAction("Counter");
	werk::BackgroundThread backgroundThread(nullptr, 1l * 1000 * 1000);

	werk::Watchdog watchdog("Watchdog", &backgroundThread.backgroundClock(), &counterAction, 30l * 1000 * 1000, 0);
	BOOST_REQUIRE_EQUAL(watchdog.interval(), 30l * 1000 * 1000);
	BOOST_REQUIRE_EQUAL(watchdog.allowedMisses(), 0);

	//Start running
	backgroundThread.addTask(&watchdog);

	//Sleep for a little more than half the frequency -- no action
	timespec delay { 0, 18l * 1000 * 1000 };
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
