
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

#include <map>
#include <string>
#include <vector>

#include "Attributes.hpp"

namespace werk
{

/**
 * Base class for any object that has a name. Since many objects have names for
 * debugging, this saves a lot of boilerplate.
 */
template <typename T>
class NamedObjectManager
{
public:
	CHECKED const std::vector<T *> objects() const { return _objects; }
	CHECKED const std::map<std::string, T *> objectsByName() const { return _objectsByName; }

	CHECKED T *get(const std::string &name) const {
		auto i = _objectsByName.find(name);
		return i == _objectsByName.end() ? nullptr : i->second;
	}

	bool add(T *object) {
		auto result = _objectsByName.insert(std::make_pair(object->name(), object));
		if (result.second) {
			_objects.push_back(object);
		}
		return result.second;
	}

protected:
	std::vector<T *> _objects;
	std::map<std::string, T *> _objectsByName;
};

}
