
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "ScopedConfig.hpp"

#include "Config.hpp"

namespace werk
{

void ScopedConfig::findScopedKey(const std::string &key, std::string &foundKey) const
{
	for (const std::string &scope : _scopes) {
		const std::string qualifiedKey = scope + key;
		const char *stringValue = _config.getStringRaw(qualifiedKey);
		if (nullptr != stringValue) {
			foundKey = qualifiedKey;
			return;
		}
	}

	foundKey = _scopes.size() == 0 ? key : _scopes[0] + key;
}

const char *ScopedConfig::getString(const std::string &key, const char *defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getString(scopedKey, defaultValue, help);
}

bool ScopedConfig::getBool(const std::string &key, bool defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getBool(scopedKey, defaultValue, help);
}

double ScopedConfig::getDouble(const std::string &key, double defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getDouble(scopedKey, defaultValue, help);
}

int64_t ScopedConfig::getInt64(const std::string &key, int64_t defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getInt64(scopedKey, defaultValue, help);
}

uint64_t ScopedConfig::getUint64(const std::string &key, uint64_t defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getUint64(scopedKey, defaultValue, help);
}

uint64_t ScopedConfig::getStorageAmount(const std::string &key, uint64_t defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getStorageAmount(scopedKey, defaultValue, help);
}

uint64_t ScopedConfig::getTimeAmount(const std::string &key, uint64_t defaultValue, const char *help) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getTimeAmount(scopedKey, defaultValue, help);
}

//List types
const char *ScopedConfig::getStrings(const std::string &key, std::vector<std::string> &values,
	const char *defaultValue, const char *help, const char *delimiters) const
{
	std::string scopedKey;
	findScopedKey(key, scopedKey);
	return _config.getStrings(scopedKey, values, defaultValue, help, delimiters);
}

}
