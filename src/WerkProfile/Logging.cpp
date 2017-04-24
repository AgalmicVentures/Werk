
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

	werk::BackgroundThread backgroundThread(nullptr, 1l * 1000 * 1000);
	FILE *file = std::fopen("/dev/null", "a");
	werk::AsyncLog *log = new werk::AsyncLog("Log", &backgroundThread.backgroundClock(), file);
	backgroundThread.addTask(log);

	//Raw logging
	werk::Profile *asyncRawProfile = new werk::Profile("AsyncLogRaw", 1000, 1000);
	profileManager.add(asyncRawProfile);
	for (size_t i = 0; i < iterations; ++i) {
		PROFILE_START(*asyncRawProfile);
		log->logRaw(werk::LogLevel::INFO, "Testing raw log");
		PROFILE_STOP(*asyncRawProfile);
	}

	//Formatted logging
	werk::Profile *asyncProfile = new werk::Profile("AsyncLog", 1000, 1000);
	profileManager.add(asyncProfile);
	for (int i = 0; i < iterations; ++i) {
		PROFILE_START(*asyncProfile);
		log->log(werk::LogLevel::INFO, "Testing log with formatting %f %d", 3.14, i);
		PROFILE_STOP(*asyncProfile);
	}

	backgroundThread.stop();
	std::fclose(file);

	if (!disableTests) {
		BOOST_CHECK_LE(asyncRawProfile->f50Statistics().average(), 600.0);
		BOOST_CHECK_LE(asyncRawProfile->f75Statistics().average(), 750.0);
		BOOST_CHECK_LE(asyncRawProfile->f95Statistics().average(), 1000.0);
		BOOST_CHECK_LE(asyncRawProfile->f99Statistics().average(), 1500.0);

		BOOST_CHECK_LE(asyncProfile->f99Statistics().average(), 4000.0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
