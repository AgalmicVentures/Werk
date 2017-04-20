
#include "Hardware.hpp"

#ifdef __MACH__
	#include <sys/types.h>
	#include <sys/sysctl.h>
#endif
#include <unistd.h>

namespace werk
{

uint64_t getPageSize()
{
    long pageSize = sysconf(_SC_PAGE_SIZE);
    if (pageSize < 0) {
        return 0;
    }

    return static_cast<uint64_t>(pageSize);
}

uint64_t getPhysicalMemorySize()
{
#ifdef __MACH__
	int name[2] = { CTL_HW, HW_MEMSIZE };
    u_int nameLength = sizeof(name) / sizeof(name[0]);
    uint64_t size;
	size_t sizeLength = sizeof(size);
    if (sysctl(name, nameLength, &size, &sizeLength, nullptr, 0) < 0) {
        return 0;
    }

	return size;
#else
    long pages = sysconf(_SC_PHYS_PAGES);
    if (pages < 0) {
        return 0;
    }

    uint64_t pageSize = getPageSize();
    return pages * pageSize;
#endif
}

size_t getProcessorCount()
{
	return static_cast<size_t>(sysconf(_SC_NPROCESSORS_CONF));
}

}
