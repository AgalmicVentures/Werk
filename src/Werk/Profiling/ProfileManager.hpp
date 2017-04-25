
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

#pragma once

#include <cstdio>
#include <map>
#include <string>

#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/Profile.hpp"

namespace werk
{

/**
 * Manages a set of named `Profile`s, including a default baseline profile.
 */
class ProfileManager
{
public:

	ProfileManager() {
		//Check the time it takes to call the timing functions
		add(&_baseProfile);
		for (uint64_t i = 0; i < 101 * 1000; ++i) {
			PROFILE_START(_baseProfile);
			PROFILE_STOP(_baseProfile);
		}
	}

	const std::map<std::string, Profile *> &profiles() const { return _profiles; }

	void add(Profile *profile) {
		_profiles[profile->name()] = profile;
	}

	void writeJson(FILE *file) const {
		for (auto i = _profiles.begin(); i != _profiles.end(); ++i) {
			i->second->writeJson(file);
		}
	}

private:
	std::map<std::string, Profile *> _profiles;

	//A profile that times the basic timer function, so that can be subtracted out
	Profile _baseProfile { "Base", 1000, 1000 } ;
};

}
