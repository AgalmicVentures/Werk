
#pragma once

#include <cstddef>

namespace werk
{

/**
 * Returns the number of processors on this system.
 */
long getProcessorCount();

/**
 * Represents a CPU mask used to set processor affinity.
 */
class CpuMask
{
public:

	CpuMask(size_t processorCount) : _processorCount(processorCount) { }

	//TODO: get, set, etc.

	//Applies the mask to this thread, returning true on success
	bool apply();

protected:
	size_t _processorCount;
};

}
