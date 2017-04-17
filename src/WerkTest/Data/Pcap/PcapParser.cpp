
#include <cmath>

#include <boost/test/unit_test.hpp>

#include "Werk/Data/Pcap/PcapParser.hpp"

BOOST_AUTO_TEST_SUITE(PcapParserTest)

BOOST_AUTO_TEST_CASE(testBasic)
{
	werk::PcapParser pcapParser;
	BOOST_REQUIRE(pcapParser.open("src/WerkTest/Data/Pcap/SSH_L3.pcap"));

	BOOST_CHECK(!pcapParser.isNanosecond());
	BOOST_CHECK_EQUAL(pcapParser.versionMajor(), 2);
	BOOST_CHECK_EQUAL(pcapParser.versionMinor(), 4);
	BOOST_CHECK_EQUAL(pcapParser.tzOffset(), 0);
	BOOST_CHECK_EQUAL(pcapParser.sigfigs(), 0);
	BOOST_CHECK_EQUAL(pcapParser.snaplen(), 0xFFFF);
	BOOST_CHECK_EQUAL(pcapParser.network(), 228);

	for (size_t i = 0; i < 21; ++i) {
		BOOST_REQUIRE(pcapParser.moveNext());
		BOOST_CHECK_EQUAL(pcapParser.length(), pcapParser.originalLength());
	}

	BOOST_REQUIRE(!pcapParser.moveNext());
}

BOOST_AUTO_TEST_SUITE_END()
