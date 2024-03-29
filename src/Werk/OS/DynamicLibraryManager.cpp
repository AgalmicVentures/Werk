
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

#include "DynamicLibraryManager.hpp"

#include <dlfcn.h>

namespace werk
{

DynamicLibrary::~DynamicLibrary()
{
	dlclose(_handle);
}

DynamicLibraryManager::~DynamicLibraryManager()
{
	//Clean up anything that's still open
	_libraries.clear();
}

DynamicLibrary *DynamicLibraryManager::load(const std::string &path)
{
	const auto i = _libraries.find(path);
	if (i != _libraries.end()) {
		return &i->second;
	}

	//Load it
	void *handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
	if (nullptr != handle) {
		const auto j = _libraries.emplace(path, handle);
		if (j.second) {
			return &j.first->second;
		}
	}

	return nullptr;
}

void DynamicLibraryManager::unload(const std::string &path)
{
	const auto i = _libraries.find(path);
	if (i == _libraries.end()) {
		return;
	}

	//Delete it which will automatically unload it
	_libraries.erase(i);
}

}
