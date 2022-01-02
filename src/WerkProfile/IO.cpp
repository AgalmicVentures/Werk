
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

#include "Werk/Profiling/ProfileManager.hpp"

extern werk::ProfileManager profileManager;
extern bool disableTests;

BOOST_AUTO_TEST_SUITE(IoTest)

BOOST_AUTO_TEST_CASE(TestDevNull)
{
	const int iterations = 200000;

	//Raw logging
	FILE *devNull = std::fopen("/dev/null", "a");
	werk::Profile *devNullProfile = new werk::Profile("WriteDevNull", 10000, 10000);
	profileManager.add(devNullProfile);
	for (size_t i = 0; i < iterations; ++i) {
		PROFILE_START(*devNullProfile);
		std::fprintf(devNull, "01234567890 123456789\n");
		PROFILE_STOP(*devNullProfile);
	}
	std::fclose(devNull);

	if (!disableTests) {
		//99th percentiles better be <= 20us, average max <= 100us
		BOOST_CHECK_LE(devNullProfile->f99Statistics().max(), 20000.0);
		BOOST_CHECK_LE(devNullProfile->maxStatistics().average(), 100000.0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
