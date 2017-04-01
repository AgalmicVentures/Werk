
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
	//TODO: error handling
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
