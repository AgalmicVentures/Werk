
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
