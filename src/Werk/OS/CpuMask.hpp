
#pragma once

#include <cstddef>

struct cpu_set_t;

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

	CpuMask(size_t processorCount);
	~CpuMask();

	void clear(size_t core);
	void set(size_t core);

	//Applies the mask to this thread, returning true on success
	bool apply();

protected:
	size_t _processorCount;
	size_t _size;
    cpu_set_t *_mask;
};

}
