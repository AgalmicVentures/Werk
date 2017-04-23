
#include <boost/test/unit_test.hpp>

#include "Werk/Profiling/ProfileManager.hpp"

extern werk::ProfileManager profileManager;
extern bool disableTests;

BOOST_AUTO_TEST_SUITE(IoTest)

BOOST_AUTO_TEST_CASE(TestDevNull)
{
	const int iterations = 25000;

	//Raw logging
	FILE *devNull = std::fopen("/dev/null", "a");
	werk::Profile *devNullProfile = new werk::Profile("WriteDevNull", 1000, 1000);
	profileManager.add(devNullProfile);
	for (size_t i = 0; i < iterations; ++i) {
		PROFILE_START(*devNullProfile);
		std::fprintf(devNull, "01234567890 123456789\n");
		PROFILE_STOP(*devNullProfile);
	}
	std::fclose(devNull);

	if (!disableTests) {
		//99th percentiles better be <= 20us, average max <= 100us
		BOOST_CHECK_LE(devNullProfile->f99Statistics().max(), 20000.0);
		BOOST_CHECK_LE(devNullProfile->maxStatistics().average(), 100000.0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
