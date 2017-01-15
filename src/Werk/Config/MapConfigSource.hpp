
#pragma once

#include <map>

#include "Config.hpp"

namespace werk
{

/**
 * Holds configuration values in a map.
 */
class MapConfigSource : public ConfigSource
{
public:

	std::map<std::string, std::string> &values() { return _values; }
	const std::map<std::string, std::string> &values() const { return _values; }

	bool reloadConfig(std::map<std::string, std::string> &values) override {
		for (auto i = _values.begin(); i != _values.end(); ++i) {
			values[i->first] = i->second;
		}
		return true;
	}

private:
	std::map<std::string, std::string> _values;
};

}
