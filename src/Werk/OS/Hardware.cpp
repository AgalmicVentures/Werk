
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
    const long pageSize = sysconf(_SC_PAGE_SIZE);
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
    const long pages = sysconf(_SC_PHYS_PAGES);
    if (pages < 0) {
        return 0;
    }

    const uint64_t pageSize = getPageSize();
    return pages * pageSize;
#endif
}

size_t getProcessorCount()
{
	return static_cast<size_t>(sysconf(_SC_NPROCESSORS_CONF));
}

}
