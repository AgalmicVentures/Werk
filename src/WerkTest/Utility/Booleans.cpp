
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

#include "Werk/Utility/Booleans.hpp"

BOOST_AUTO_TEST_SUITE(BooleansTest)

BOOST_AUTO_TEST_CASE(TestParseBool)
{
	BOOST_CHECK(werk::parseBool("t"));
	BOOST_CHECK(werk::parseBool("T"));
	BOOST_CHECK(werk::parseBool("true"));
	BOOST_CHECK(werk::parseBool("True"));

	BOOST_CHECK(!werk::parseBool("f"));
	BOOST_CHECK(!werk::parseBool("F"));
	BOOST_CHECK(!werk::parseBool("false"));
	BOOST_CHECK(!werk::parseBool("False"));

	BOOST_CHECK(!werk::parseBool("asdf"));
}

BOOST_AUTO_TEST_CASE(TestTryParseBool)
{
	BOOST_CHECK(werk::tryParseBool("t") == std::optional<bool>(true));
	BOOST_CHECK(werk::tryParseBool("T") == std::optional<bool>(true));
	BOOST_CHECK(werk::tryParseBool("true") == std::optional<bool>(true));
	BOOST_CHECK(werk::tryParseBool("True") == std::optional<bool>(true));

	BOOST_CHECK(werk::tryParseBool("f") == std::optional<bool>(false));
	BOOST_CHECK(werk::tryParseBool("F") == std::optional<bool>(false));
	BOOST_CHECK(werk::tryParseBool("false") == std::optional<bool>(false));
	BOOST_CHECK(werk::tryParseBool("False") == std::optional<bool>(false));

	BOOST_CHECK(werk::tryParseBool("asdf") == std::nullopt);
}

BOOST_AUTO_TEST_SUITE_END()
