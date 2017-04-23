
#include <boost/test/unit_test.hpp>

#include "Werk/OS/CpuMask.hpp"
#include "Werk/OS/Hardware.hpp"

BOOST_AUTO_TEST_SUITE(CpuMaskTest)

BOOST_AUTO_TEST_CASE(TestCpuMask)
{
	size_t processorCount = werk::getProcessorCount();
	werk::CpuMask mask(processorCount);
	for (size_t i = 0; i < processorCount; ++i) {
		mask.set(i);
	}

	//Here, this may not work since some platforms are not supported
	(void) mask.apply();
}

BOOST_AUTO_TEST_SUITE_END()
