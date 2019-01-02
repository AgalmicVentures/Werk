
/*
 * Copyright (c) 2015-2019 Agalmic Ventures LLC (www.agalmicventures.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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
