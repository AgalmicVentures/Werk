
#define BOOST_TEST_MODULE WerkProfile
#include <boost/test/included/unit_test.hpp>

#include <string>

#include "Werk/Profiling/ProfileManager.hpp"

werk::ProfileManager profileManager;
bool disableTests = false;

struct GlobalFixture
{
	GlobalFixture() {
		//Optionally disable testing (e.g. for valgrind)
		if (boost::unit_test::framework::master_test_suite().argc > 1) {
			const std::string flag(boost::unit_test::framework::master_test_suite().argv[1]);
			if (flag == "--disable-tests") {
				disableTests = true;
			}
		}
	}
    ~GlobalFixture() {
		profileManager.writeJson(stdout);
	}
};

BOOST_GLOBAL_FIXTURE( GlobalFixture );

