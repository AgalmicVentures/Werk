
#pragma once

#include <boost/algorithm/string/join.hpp>

#include "Werk/Commands/Command.hpp"

#include "Config.hpp"

namespace werk
{

/**
 * Command that reloads a configuration.
 */
class ReloadConfigCommand : public Command
{
public:

	ReloadConfigCommand(Config &config) :
		Command("Sets the configuration reload flag."),
		_config(config) { }

	virtual bool execute(const std::vector<std::string> &/*rguments*/) {
		_config.reloadConfig();
		return true;
	}

private:
	Config &_config;
};

}
