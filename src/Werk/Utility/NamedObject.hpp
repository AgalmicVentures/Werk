
#pragma once

#include <string>

namespace werk
{

/**
 * Base class for any object that has a name. Since many objects have names for
 * debugging, this saves a lot of boilerplate.
 */
class NamedObject
{
public:
	NamedObject(const std::string &name) : _name(name) { }

	const std::string &name() const { return _name; }

private:
	std::string _name;
};

}
