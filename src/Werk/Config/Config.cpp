
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
	if (stringValue == nullptr) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %s [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s%s%s",
		key.c_str(),
		stringValue,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return stringValue;
}

bool Config::getBool(const std::string &key, bool defaultValue, const char *help) const
{
	const ConfigValuesT * const values = _values;
	auto i = values->find(key);

	if (i == values->end()) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %s [DEFAULT]%s%s",
			key.c_str(),
			defaultValue ? "true" : "false",
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s%s%s",
		key.c_str(),
		i->second.c_str(),
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return i->second == "true" || i->second == "True";
}

double Config::getDouble(const std::string &key, double defaultValue, const char *help) const
{
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %f [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	double value = std::stod(stringValue);
	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%f)%s%s",
		key.c_str(),
		stringValue, value,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return value;
}

int64_t Config::getInt64(const std::string &key, int64_t defaultValue, const char *help) const
{
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %" PRIi64 " [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	int64_t value = std::stoll(stringValue);
	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIi64 ")%s%s",
		key.c_str(),
		stringValue, value,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return value;
}

uint64_t Config::getUint64(const std::string &key, uint64_t defaultValue, const char *help) const
{
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %" PRIu64 " [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	uint64_t value = std::stoull(stringValue);
	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s",
		key.c_str(),
		stringValue, value,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return value;
}

uint64_t Config::getStorageAmount(const std::string &key, uint64_t defaultValue, const char *help) const
{
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %" PRIu64 " [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	uint64_t value = parseUnits(stringValue, STORAGE_UNITS);
	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s",
		key.c_str(),
		stringValue, value,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return value;
}

uint64_t Config::getTimeAmount(const std::string &key, uint64_t defaultValue, const char *help) const
{
	const char *stringValue = getStringRaw(key);
	if (stringValue == nullptr) {
		_log->log(LogLevel::CONFIG, "<Config> [%s] = %" PRIu64 " [DEFAULT]%s%s",
			key.c_str(),
			defaultValue,
			help == nullptr ? "" : " -- ",
			help == nullptr ? "" : help);
		return defaultValue;
	}

	uint64_t value = parseUnits(stringValue, TIME_UNITS);
	_log->log(LogLevel::CONFIG, "<Config> [%s] = %s (%" PRIu64 ")%s%s",
		key.c_str(),
		stringValue, value,
		help == nullptr ? "" : " -- ",
		help == nullptr ? "" : help);
	return value;
}

}
