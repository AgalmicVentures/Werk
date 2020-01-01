
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
