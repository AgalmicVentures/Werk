
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
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

#define BOOST_TEST_MODULE WerkProfile
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <string>

#include "Werk/Profiling/ProfileManager.hpp"

werk::ProfileManager profileManager;
bool disableTests = false;

struct GlobalFixture
{
	GlobalFixture() {
		//Optionally disable testing (e.g. for valgrind)
		if (boost::unit_test::framework::master_test_suite().argc > 1) {
			const std::string flag(boost::unit_test::framework::master_test_suite().argv[1]);
			if (flag == "--disable-tests") {
				std::cout << "Disabling test pass/fail..." << std::endl;
				disableTests = true;
			}
		}
	}
    ~GlobalFixture() {
		profileManager.writeJson(stdout);
	}
};

BOOST_GLOBAL_FIXTURE( GlobalFixture );

