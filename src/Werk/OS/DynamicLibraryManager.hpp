
#pragma once

#include <map>
#include <string>

namespace werk
{

/**
 * Represents a dynamic library, see below for use.
 */
class DynamicLibrary
{
public:
	DynamicLibrary(void *handle) : _handle(handle) { }
	~DynamicLibrary();

	void *getSymbol(const char *name);

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

	DynamicLibrary *load(const std::string &path);
	void unload(const std::string &path);

protected:
	std::map<std::string, DynamicLibrary> _libraries;
};

}
