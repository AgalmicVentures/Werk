
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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
