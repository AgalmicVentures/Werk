
#include <boost/test/unit_test.hpp>

#include "Werk/OS/CpuMask.hpp"
#include "Werk/OS/Hardware.hpp"

BOOST_AUTO_TEST_SUITE(CpuMaskTest)

BOOST_AUTO_TEST_CASE(TestCpuMask)
{
	werk::CpuMask mask(werk::getProcessorCount());
	mask.set(0);
	mask.set(1);

	mask.apply();
}

BOOST_AUTO_TEST_SUITE_END()
