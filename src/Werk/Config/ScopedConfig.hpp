
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
#include "Werk/Utility/Attributes.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class Config;

/**
 * Adapter for the Config class that allows keys to be read from a number of scopes,
 * allowing for cascading configurations.
 */
class ScopedConfig
{
public:

	ScopedConfig(Config &config) : _config(config) { }
	ScopedConfig(Config &config, const std::string &scope) : ScopedConfig(config)
	{
		_scopes.push_back(scope);
	}
	ScopedConfig(Config &config, const std::string &scope1, const std::string scope2) :
		ScopedConfig(config)
	{
		_scopes.push_back(scope1);
		_scopes.push_back(scope2);
	}
	ScopedConfig(Config &config, const std::string &scope1, const std::string scope2, const std::string scope3) :
		ScopedConfig(config)
	{
		_scopes.push_back(scope1);
		_scopes.push_back(scope2);
		_scopes.push_back(scope3);
	}
	ScopedConfig(Config &config, const std::string &scope1, const std::string scope2, const std::string scope3, const std::string scope4) :
		ScopedConfig(config)
	{
		_scopes.push_back(scope1);
		_scopes.push_back(scope2);
		_scopes.push_back(scope3);
		_scopes.push_back(scope4);
	}

	CHECKED std::vector<std::string> &scopes() { return _scopes; }
	CHECKED const std::vector<std::string> &scopes() const { return _scopes; }

	//Basic types
	CHECKED const char *getString(const std::string &key, const char *defaultValue=nullptr, const char *help=nullptr) const;
	CHECKED bool getBool(const std::string &key, bool defaultValue=false, const char *help=nullptr) const;
	CHECKED double getDouble(const std::string &key, double defaultValue=0, const char *help=nullptr) const;
	CHECKED int64_t getInt64(const std::string &key, int64_t defaultValue=0, const char *help=nullptr) const;
	CHECKED uint64_t getUint64(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const;
	CHECKED uint64_t getStorageAmount(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const;
	CHECKED uint64_t getTimeAmount(const std::string &key, uint64_t defaultValue=0, const char *help=nullptr) const;

	//List types
	const char *getStrings(const std::string &key, std::vector<std::string> &values,
		const char *defaultValue=nullptr, const char *help=nullptr, const char *delimiters=",") const;

protected:
	//Config
	Config &_config;
	std::vector<std::string> _scopes;

	void findScopedKey(const std::string &key, std::string &specificKey, std::string &foundKey) const;
};

}
