
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

#include <boost/test/unit_test.hpp>

#include "Werk/Profiling/ProfileManager.hpp"

extern werk::ProfileManager profileManager;

BOOST_AUTO_TEST_SUITE(AllocationTest)

BOOST_AUTO_TEST_CASE(TestNewDelete)
{
	//N-byte new + delete
	const int iterations = 200000;
	const int sizes[] = {1, 16, 256, 2048, 65536, 1024 * 1024};
	for (const int size : sizes) {
		werk::Profile *newDeleteProfile = new werk::Profile(std::string("NewDelete") + std::to_string(size), 10000, 10000);

		profileManager.add(newDeleteProfile);
		for (size_t i = 0; i < iterations; ++i) {
			PROFILE_START(*newDeleteProfile);
			uint8_t *buffer = new uint8_t[size];
			delete [] buffer;
			PROFILE_STOP(*newDeleteProfile);
		}
	}
}

BOOST_AUTO_TEST_CASE(TestNewThenDelete)
{
	//K N-byte news then deletes
	const int allocations = 2000;
	const int sizes[] = {1, 16, 256, 2048, 65536, 1024 * 1024};
	uint8_t **buffers = new uint8_t*[allocations];
	for (const int size : sizes) {
		werk::Profile *newProfile = new werk::Profile(std::string("NewDelete_New") + std::to_string(size), 1000, 1000);
		werk::Profile *deleteProfile = new werk::Profile(std::string("NewDelete_Delete") + std::to_string(size), 1000, 1000);

		profileManager.add(newProfile);
		for (size_t i = 0; i < allocations; ++i) {
			PROFILE_START(*newProfile);
			buffers[i] = new uint8_t[size];
			PROFILE_STOP(*newProfile);
		}

		profileManager.add(deleteProfile);
		for (size_t i = 0; i < allocations; ++i) {
			PROFILE_START(*deleteProfile);
			delete [] buffers[i];
			PROFILE_STOP(*deleteProfile);
		}
	}
	delete [] buffers;
}

BOOST_AUTO_TEST_SUITE_END()
