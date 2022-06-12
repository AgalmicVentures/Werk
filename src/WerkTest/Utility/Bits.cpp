
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <boost/test/unit_test.hpp>

#include "Werk/Utility/Bits.hpp"

BOOST_AUTO_TEST_SUITE(BitsTest)

BOOST_AUTO_TEST_CASE(TestBitmask)
{
	BOOST_CHECK_EQUAL(werk::bitmask<uint64_t>(0), 0x00);
	BOOST_CHECK_EQUAL(werk::bitmask<uint64_t>(1), 0x01);
	BOOST_CHECK_EQUAL(werk::bitmask<uint64_t>(2), 0x03);
	BOOST_CHECK_EQUAL(werk::bitmask<uint64_t>(3), 0x07);
	BOOST_CHECK_EQUAL(werk::bitmask<uint64_t>(4), 0x0F);
	BOOST_CHECK_EQUAL(werk::bitmask<uint64_t>(5), 0x1F);
}

BOOST_AUTO_TEST_CASE(TestRoundUpPow2)
{
	//Simple shifting
	uint64_t x = 0x0E;
	uint64_t y = 0x10;
	for (size_t i = 0; i < 32; ++i) {
		BOOST_CHECK_EQUAL(werk::roundUpPow2(x), y);
		x *= 0x10;
		y *= 0x10;
	}

	//Types matter
	BOOST_CHECK_EQUAL(werk::roundUpPow2(0xDEADBEEF), 0);
	BOOST_CHECK_EQUAL(werk::roundUpPow2<uint32_t>(0xDEADBEEF), 0);
	BOOST_CHECK_EQUAL(werk::roundUpPow2<uint64_t>(0xDEADBEEF), 0x100000000);
}

BOOST_AUTO_TEST_CASE(TestPopCount64)
{
	for (size_t i = 0; i < 64; ++i) {
		uint64_t value = werk::bitmask<uint64_t>(i);
		BOOST_CHECK_EQUAL(werk::popCount64(value), i);
	}
}

BOOST_AUTO_TEST_SUITE_END()
