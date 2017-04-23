
#pragma once

#include <cstddef>
#include <sched.h>

#include "Werk/Utility/Attributes.hpp"

#ifdef __MACH__
	struct cpu_set_t;
#endif

namespace werk
{

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
	CHECKED bool apply();

protected:
	const size_t _processorCount;
	size_t _size;
	cpu_set_t *_mask;
};

}
