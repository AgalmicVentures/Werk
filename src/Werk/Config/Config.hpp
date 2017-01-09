
#pragma once

#include <cstdint>
#include <limits>
#include <string>

namespace werk
{

//Abstract config class to allow integration with other configuration systems.
class Config
{
public:

	//Basic value-as-string accessor - this is the method that every inheriting class must override
	virtual const char *getString(const std::string &key, const char *defaultValue=nullptr) const = 0;

	//Other basic types
	virtual double getDouble(const std::string &key, double defaultValue=0) const {
		const char *stringValue = getString(key);
		return stringValue != nullptr ? std::stod(stringValue) : defaultValue;
	}
	virtual int64_t getInt64(const std::string &key, int64_t defaultValue=0) const {
		const char *stringValue = getString(key);
		return stringValue != nullptr ? std::stoll(stringValue) : defaultValue;
	}
	virtual uint64_t getUint64(const std::string &key, uint64_t defaultValue=0) const {
		const char *stringValue = getString(key);
		return stringValue != nullptr ? std::stoull(stringValue) : defaultValue;
	}

	//TODO: more complex types e.g. time
};

}
