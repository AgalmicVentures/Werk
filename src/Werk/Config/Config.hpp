
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

#include <atomic>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <limits>
#include <map>
#include <string>
#include <vector>

#include "Werk/Logging/Log.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class Config;
typedef std::map<std::string, std::string> ConfigValuesT;

/**
 * Abstract class representing any object that consumes configuration information.
 */
class Configurable
{
public:
	//Pass the Config object so
	virtual bool reloadConfig(const Config &config) = 0;
};

/**
 * Abstract class that represents any object that provides configuration information --
 * for example, an ini file or a database connection.
 */
class ConfigSource
{
public:
	virtual bool reloadConfig(std::map<std::string, std::string> &values) = 0;
};

/**
 * Configuration class which can read from multiple `ConfigSource`s (in the background on the IO thread),
 * then propagate updates to a number of `Configurable`s.
 */
class Config : public Action
{
public:

	Config(const std::string &name, Log *log) :
		Action(name + "_Reloader"), _log(log), _reloadConfigAction(name + "_ReloadAction", _reloadConfig)
	{
		_values.store(&_values1);
	}

	void addConfigSource(ConfigSource *configSource) {
		if (nullptr == configSource) {
			_log->log(LogLevel::ERROR, "<Config> Tried to add null config source");
			return;
		}
		_configSources.push_back(configSource);
	}
	void addConfigurable(Configurable *configurable) {
		if (nullptr == configurable) {
			_log->log(LogLevel::ERROR, "<Config> Tried to add null configurable");
			return;
		}
		_configurables.push_back(configurable);
	}

	//This exists so the log can be swapped from stdout/stderr to a file once a config is read, if necessary
	void setLog(Log *log) {
		assert(nullptr != log);
		_log = log;
	}

	//Flags the config to be reloaded in the background
	void reloadConfig() { _reloadConfig.set(); }
	Action *getReloadConfigAction() { return &_reloadConfigAction; }

	//Run in the background to reload
	void execute() override;

	//Run on whatever thread the configurables live on
	void reloadConfigurables();

	//Basic value-as-string accessor - this is the method that every inheriting class must override (return nullptr if the key is missing)
	const char *getStringRaw(const std::string &key) const {
		const ConfigValuesT * const values = _values;
		auto i = values->find(key);
		return i == values->end() ? nullptr : i->second.c_str();
	}

	//Basic types
	const char *getString(const std::string &key, const char *defaultValue=nullptr, const char *help=nullptr) const;
	bool getBool(const std::string &key, bool defaultValue=false, const char *help=nullptr) const;
	double getDouble(const std::string &key, double defaultValue=0, const char *help=nullptr) const;
	int64_t getInt64(const std::string &key, int64_t defaultValue=0, const char *help=nullptr) const;
	uint64_t getUint64(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const;
	uint64_t getStorageAmount(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const;
	uint64_t getTimeAmount(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const;

	//List types
	const char *getStrings(const std::string &key, std::vector<std::string> &values,
		const char *defaultValue=nullptr, const char *help=nullptr, const char *delimiters=",") const;

protected:
	//Config
	Log *_log;
	std::vector<ConfigSource *> _configSources;
	std::vector<Configurable *> _configurables;

	//Helpers
	SetLatchAction<std::atomic<bool> > _reloadConfigAction;

	//State
	Latch<std::atomic<bool> > _reloadConfig;
	Latch<std::atomic<bool> > _changed;
	std::atomic<ConfigValuesT *> _values;
	ConfigValuesT _values1;
	ConfigValuesT _values2;
};

}
