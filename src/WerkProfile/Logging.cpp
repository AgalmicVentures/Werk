
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

#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Logging/Log.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

extern werk::ProfileManager profileManager;
extern bool disableTests;

BOOST_AUTO_TEST_SUITE(LoggingTest)

BOOST_AUTO_TEST_CASE(TestAsyncLog)
{
	const int iterations = 200000;

	werk::BackgroundThread backgroundThread("Background", nullptr, 1l * 1000 * 1000);
	FILE *file = std::fopen("/dev/null", "a");
	werk::AsyncLog *log = new werk::AsyncLog("Log", &backgroundThread.backgroundClock(), file);
	backgroundThread.addTask(log);

	//Raw logging
	werk::Profile *asyncRawProfile = new werk::Profile("AsyncLogRaw", 10000, 10000);
	profileManager.add(asyncRawProfile);
	for (size_t i = 0; i < iterations; ++i) {
		PROFILE_START(*asyncRawProfile);
		log->logRaw(werk::LogLevel::INFO, "Testing raw log");
		PROFILE_STOP(*asyncRawProfile);
	}

	//Formatted logging
	werk::Profile *asyncProfile = new werk::Profile("AsyncLog", 10000, 10000);
	profileManager.add(asyncProfile);
	for (int i = 0; i < iterations; ++i) {
		PROFILE_START(*asyncProfile);
		log->log(werk::LogLevel::INFO, "Testing log with formatting %f %d", 3.14, i);
		PROFILE_STOP(*asyncProfile);
	}

	backgroundThread.stop();
	std::fclose(file);

	if (!disableTests) {
		BOOST_CHECK_LE(asyncRawProfile->f75Statistics().average(), 750.0);
		BOOST_CHECK_LE(asyncRawProfile->f95Statistics().average(), 1000.0);
		BOOST_CHECK_LE(asyncRawProfile->f99Statistics().average(), 1500.0);

		BOOST_CHECK_LE(asyncProfile->f99Statistics().average(), 4000.0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
