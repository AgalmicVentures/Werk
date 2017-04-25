
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

#include "CpuMask.hpp"

#include <unistd.h>

namespace werk
{

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
	CPU_FREE(_mask);
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
