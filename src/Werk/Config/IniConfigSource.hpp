
#pragma once

#include "Config.hpp"

namespace werk
{

/**
 * Loads a configuration from an ini file.
 */
class IniConfigSource : public ConfigSource
{
public:

	IniConfigSource(const std::string &path) : _path(path) { }

	bool reloadConfig(std::map<std::string, std::string> &values) override;

private:
	const std::string _path;
};

}
