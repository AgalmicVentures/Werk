
#include "Config.hpp"

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
void Config::reloadConfigurables()
{
	//Don't reload unless changed
	if (!_changed.value()) {
		return;
	}
	_changed.reset();

	for (Configurable *configurable : _configurables) {
		configurable->reloadConfig(*this);
	}
}

//Basic types
const char *Config::getString(const std::string &key, const char *defaultValue, const char *help) const
{
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
		key.c_str(),
		stringValue,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%s%s%s}",
		key.c_str(),
		quoteChar,
		nullptr == stringValue ? "null" : stringValue,
		quoteChar);
	return stringValue;
}

bool Config::getBool(const std::string &key, bool defaultValue, const char *help) const
{
	bool value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const ConfigValuesT * const values = _values;
	auto i = values->find(key);
	if (i != values->end()) {
		value = i->second == "true" || i->second == "True";
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s%s%s",
		key.c_str(),
		i->second.c_str(),
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%s}",
		key.c_str(),
		value ? "true" : "false");
	return value;
}

double Config::getDouble(const std::string &key, double defaultValue, const char *help) const
{
	double value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = std::stod(stringValue);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%f)%s%s%s",
		key.c_str(),
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%f}",
		key.c_str(),
		value);
	return value;
}

int64_t Config::getInt64(const std::string &key, int64_t defaultValue, const char *help) const
{
	int64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = std::stoll(stringValue);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIi64 ")%s%s%s",
		key.c_str(),
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%" PRIi64 "}",
		key.c_str(),
		value);
	return value;
}

uint64_t Config::getUint64(const std::string &key, uint64_t defaultValue, const char *help) const
{
	uint64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = std::stoull(stringValue);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s%s",
		key.c_str(),
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%" PRIu64 "}",
		key.c_str(),
		value);
	return value;
}

uint64_t Config::getStorageAmount(const std::string &key, uint64_t defaultValue, const char *help) const
{
	uint64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = parseUnits(stringValue, STORAGE_UNITS);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s%s",
		key.c_str(),
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%" PRIu64 "}",
		key.c_str(),
		value);
	return value;
}

uint64_t Config::getTimeAmount(const std::string &key, uint64_t defaultValue, const char *help) const
{
	uint64_t value = defaultValue;
	const char *defaultLogValue = " [DEFAULT]";

	const char *stringValue = getStringRaw(key);
	if (stringValue != nullptr) {
		value = parseUnits(stringValue, TIME_UNITS);
		defaultLogValue = "";
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s%s",
		key.c_str(),
		stringValue, value,
		defaultLogValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	_log->log(LogLevel::JSON, "{\"type\":\"config\",\"key\":\"%s\",\"value\":%" PRIu64 "}",
		key.c_str(),
		value);
	return value;
}

}
