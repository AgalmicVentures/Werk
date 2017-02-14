
#include "CpuMask.hpp"

#include <unistd.h>

namespace werk
{

long getProcessorCount()
{
	return sysconf(_SC_NPROCESSORS_CONF);
}

CpuMask::CpuMask(size_t processorCount) : _processorCount(processorCount)
{
#ifndef __MACH__
	_size = CPU_ALLOC_SIZE(processorCount);
	_mask = CPU_ALLOC(processorCount);
    CPU_ZERO_S(_size, _mask);
#endif
}

CpuMask::~CpuMask()
{
#ifndef __MACH__
	CPU_FREE(msk->mask);
#endif
}

void CpuMask::clear(size_t core)
{
#ifndef __MACH__
    CPU_CLR_S(core, _size, _mask);
#else
	(void) core;
#endif
}

void CpuMask::set(size_t core)
{
#ifndef __MACH__
    CPU_SET_S(core, _size, _mask);
#else
	(void) core;
#endif
}

bool CpuMask::apply()
{
#ifndef __MACH__
	return 0 != sched_setaffinity(0, _size, _mask);
#else
	return false;
#endif
}

}
