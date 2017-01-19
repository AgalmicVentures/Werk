
#pragma once

#include "Werk/Utility/Action.hpp"

#include "ProfileManager.hpp"

namespace werk
{

/**
 * Writes the profiles from a given profile manager to a given path.
 */
class WriteProfilesAction : public Action
{
public:
	WriteProfilesAction(const std::string &name, Log *log, const ProfileManager &profileManager, const std::string &path) :
		Action(name), _log(log), _profileManager(profileManager), _path(path) { }

	void execute() override {
		FILE *file = std::fopen(_path.c_str(), "w");
		if (nullptr == file) {
			_log->log(LogLevel::INFO, "Failed to write profiles to %s", _path.c_str());
			return;
		}

		_profileManager.writeJson(file);
		std::fclose(file);
	}

private:
	Log *_log;
	const ProfileManager &_profileManager;
	const std::string _path;
};

}
