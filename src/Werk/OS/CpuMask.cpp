
#include "CpuMask.hpp"

#include <sched.h>
#include <unistd.h>

namespace werk
{

long getProcessorCount()
{
	return sysconf(_SC_NPROCESSORS_CONF);
}

bool CpuMask::apply()
{
	//TODO
	return true;
}

}
