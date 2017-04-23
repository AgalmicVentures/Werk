
#include <boost/test/unit_test.hpp>

#include "Werk/Profiling/ProfileManager.hpp"

extern werk::ProfileManager profileManager;

BOOST_AUTO_TEST_SUITE(AllocationTest)

BOOST_AUTO_TEST_CASE(TestNewDelete)
{
	//N-byte new + delete
	const int iterations = 100000;
	const int sizes[] = {1, 16, 256, 2048, 65536, 1024 * 1024};
	for (const int size : sizes) {
		werk::Profile *newDeleteProfile = new werk::Profile(std::string("NewDelete") + std::to_string(size), 1000, 1000);

		profileManager.add(newDeleteProfile);
		for (size_t i = 0; i < iterations; ++i) {
			PROFILE_START(*newDeleteProfile);
			uint8_t *buffer = new uint8_t[size];
			delete [] buffer;
			PROFILE_STOP(*newDeleteProfile);
		}
	}
}

BOOST_AUTO_TEST_CASE(TestNewThenDelete)
{
	//K N-byte news then deletes
	const int allocations = 1000;
	const int sizes[] = {1, 16, 256, 2048, 65536, 1024 * 1024};
	uint8_t **buffers = new uint8_t*[allocations];
	for (const int size : sizes) {
		werk::Profile *newProfile = new werk::Profile(std::string("NewDelete_New") + std::to_string(size), 1000, 1000);
		werk::Profile *deleteProfile = new werk::Profile(std::string("NewDelete_Delete") + std::to_string(size), 1000, 1000);

		profileManager.add(newProfile);
		for (size_t i = 0; i < allocations; ++i) {
			PROFILE_START(*newProfile);
			buffers[i] = new uint8_t[size];
			PROFILE_STOP(*newProfile);
		}

		profileManager.add(deleteProfile);
		for (size_t i = 0; i < allocations; ++i) {
			PROFILE_START(*deleteProfile);
			delete [] buffers[i];
			PROFILE_STOP(*deleteProfile);
		}
	}
	delete [] buffers;
}

BOOST_AUTO_TEST_SUITE_END()
