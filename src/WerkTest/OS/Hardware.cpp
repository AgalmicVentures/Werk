
#include <boost/test/unit_test.hpp>

#include "Werk/OS/Hardware.hpp"

BOOST_AUTO_TEST_SUITE(HardwareTest)

BOOST_AUTO_TEST_CASE(TestGetPageSize)
{
	uint64_t pageSize = werk::getPageSize();

	//Anything less than 4K won't be adequate anyway
	BOOST_REQUIRE(pageSize >= 4 * 1024);

	//And pages should come in at least 1K chunks
	BOOST_CHECK_EQUAL(pageSize % 1024, 0);
}

BOOST_AUTO_TEST_CASE(TestGetPhysicalMemorySize)
{
	size_t memorySize = werk::getPhysicalMemorySize();

	//Anything less than 64M won't be adequate anyway
	BOOST_REQUIRE(memorySize >= 64 * 1024 * 1024);

	//And memory should come in at least 1K chunks
	BOOST_CHECK_EQUAL(memorySize % 1024, 0);
}

BOOST_AUTO_TEST_CASE(TestGetProcessorCount)
{
	size_t processorCount = werk::getProcessorCount();
	BOOST_REQUIRE(processorCount >= 1);
}

BOOST_AUTO_TEST_SUITE_END()
