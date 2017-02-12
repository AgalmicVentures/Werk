
#include <boost/test/unit_test.hpp>

#include "Werk/OS/CpuMask.hpp"

BOOST_AUTO_TEST_SUITE(CpuMaskTest)

BOOST_AUTO_TEST_CASE(TestGetProcessorCount)
{
	size_t processorCount = werk::getProcessorCount();
	BOOST_REQUIRE(processorCount >= 1);
}

BOOST_AUTO_TEST_SUITE_END()
