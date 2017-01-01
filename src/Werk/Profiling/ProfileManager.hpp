
#pragma once

#include <cstdio>
#include <map>
#include <string>

#include "Werk/Profiling/Profile.hpp"

namespace werk
{

class ProfileManager
{
public:

	const std::map<std::string, Profile *> &profiles() const { return _profiles; }

	void register(Profile *profile) {
		_profiles[profile->name()] = profile;
	}

	void writeJson(FILE *file) {
		for (auto i = _profiles.begin(); i != _profiles.end(); ++i) {
			i->second->writeJson(file);
		}
	}

private:
	std::map<std::string, Profile *> _profiles;
};

}
