
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

#include "Config.hpp"

#include <boost/algorithm/string.hpp>

#include "Werk/Utility/Booleans.hpp"
#include "Werk/Utility/Units.hpp"

namespace werk
{

//This runs on the background thread
void Config::execute()
{
	//Don't reload unless asked
	if (!_reloadConfig.value()) {
		return;
	}
	_reloadConfig.reset();

	//Select which values dictionary is not active
	ConfigValuesT &newValues = _values.load() == &_values1 ? _values2 : _values1;
	newValues.clear();

	//Reload all configs
	for (ConfigSource *configSource : _configSources) {
		if (!configSource->reloadConfig(newValues)) {
			_log->logRaw(LogLevel::ERROR, "Failed to load config");
		}
	}

	//Swap the configs atomically
	_values.store(&newValues);
	_changed.set();
}

//Run on whatever thread the configurables live on
bool Config::reloadConfigurables()
{
	//Don't reload unless changed
	if (!_changed.value()) {
		return true;
	}
	_changed.reset();

	bool success = true;
	for (Configurable *configurable : _configurables) {
		if (!configurable->reloadConfig(*this)) {
			success = false;
		}
	}
	return success;
}

//Basic types
const char *Config::getString(const std::string &key, const char *defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	const char *stringValue = getStringRaw(key);
	const char *defaultLogValue = "";
	const char *quoteChar = "\"";
	if (nullptr == stringValue) {
		stringValue = defaultValue;
		defaultLogValue = " [DEFAULT]";
		if (nullptr == stringValue) {
			quoteChar = "";
		}
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s%s%s%s",
		forKey,
		stringValue,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%s%s%s}",
		key.c_str(),
		forKey,
		quoteChar,
		nullptr == stringValue ? "null" : stringValue,
		quoteChar);
	return stringValue;
}

bool Config::getBool(const std::string &key, bool defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	bool value = defaultValue;
	const char *stringValue = value ? "true" : "false";
	const char *defaultLogValue = " [DEFAULT]";

	const ConfigValuesT * const values = _values;
	auto i = values->find(key);
	if (i != values->end()) {
		value = parseBool(i->second);
		stringValue = i->second.c_str();
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s%s%s%s",
		forKey,
		stringValue,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%s}",
		key.c_str(),
		forKey,
		value ? "true" : "false");
	return value;
}

double Config::getDouble(const std::string &key, double defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	double value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = std::stod(stringValue);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%f)%s%s%s",
		forKey,
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%f}",
		key.c_str(),
		forKey,
		value);
	return value;
}

int64_t Config::getInt64(const std::string &key, int64_t defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	int64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = std::stoll(stringValue);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIi64 ")%s%s%s",
		forKey,
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%" PRIi64 "}",
		key.c_str(),
		forKey,
		value);
	return value;
}

uint64_t Config::getUint64(const std::string &key, uint64_t defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	uint64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = std::stoull(stringValue);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s%s",
		forKey,
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%" PRIu64 "}",
		key.c_str(),
		forKey,
		value);
	return value;
}

uint64_t Config::getStorageAmount(const std::string &key, uint64_t defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	uint64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = parseUnits(stringValue, STORAGE_UNITS);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s%s",
		forKey,
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%" PRIu64 "}",
		key.c_str(),
		forKey,
		value);
	return value;
}

uint64_t Config::getTimeAmount(const std::string &key, uint64_t defaultValue,
	const char *help, const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	uint64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = parseUnits(stringValue, TIME_UNITS);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s%s",
		forKey,
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%" PRIu64 "}",
		key.c_str(),
		forKey,
		value);
	return value;
}

const char *Config::getStrings(const std::string &key, std::vector<std::string> &values,
	const char *defaultValue, const char *help, const char *delimiters,
	const char *specificKey) const
{
	const char *forKey = nullptr != specificKey ? specificKey : key.c_str();

	const char *stringValue = getStringRaw(key);
	const char *defaultLogValue = "";
	const char *quoteChar = "\"";
	if (nullptr == stringValue) {
		stringValue = defaultValue;
		defaultLogValue = " [DEFAULT]";
		if (nullptr == stringValue) {
			quoteChar = "";
		}
	}

	if (nullptr != stringValue) {
		boost::split(values, stringValue, boost::is_any_of(delimiters));
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s%s%s%s",
		forKey,
		stringValue,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"forKey\":\"%s\",\"value\":%s%s%s}",
		key.c_str(),
		forKey,
		quoteChar,
		nullptr == stringValue ? "null" : stringValue,
		quoteChar);
	return stringValue;
}

}
