
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <cassert>
#include <map>
#include <string>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Represents a dynamic library, see below for use.
 */
class DynamicLibrary
{
public:
	DynamicLibrary(void *handle) : _handle(handle) {
		assert(nullptr != handle);
	}
	~DynamicLibrary();

	CHECKED void *getSymbol(const char *name);

protected:
	void *_handle;
};

/**
 * Loads dynamic libraries and caches them. Foundational infrastructure for
 * capabilities like on-the-fly code generation, compilation, and loading.
 */
class DynamicLibraryManager
{
public:

	~DynamicLibraryManager();

	CHECKED DynamicLibrary *load(const std::string &path);
	void unload(const std::string &path);

protected:
	std::map<std::string, DynamicLibrary> _libraries;
};

}
