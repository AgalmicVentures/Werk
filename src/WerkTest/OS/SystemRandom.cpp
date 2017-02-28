
#include <boost/test/unit_test.hpp>

#include "Werk/OS/SystemRandom.hpp"

BOOST_AUTO_TEST_SUITE(SystemRandomTest)

BOOST_AUTO_TEST_CASE(TestBasic)
{
	werk::SystemRandom r;

	uint8_t buffer[65536];
	BOOST_REQUIRE(r.getBytes(buffer, sizeof(buffer)));

	uint8_t uint8Value;
	BOOST_REQUIRE(r.getUint8(uint8Value));

	uint64_t uint64Value;
	BOOST_REQUIRE(r.getUint64(uint64Value));

	int8_t int8Value;
	BOOST_REQUIRE(r.getInt8(int8Value));
}

BOOST_AUTO_TEST_SUITE_END()
