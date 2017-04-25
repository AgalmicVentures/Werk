
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

#pragma once

#ifndef __MACH__
#include <time.h>
#else
#include <sys/time.h>
#endif

namespace werk {

inline uint64_t epochTime()
{
#ifdef __MACH__
	struct timeval t;
	if (0 != gettimeofday(&t, nullptr)) {
		return 0;
	}

	//Only microsecond resolution, unfortunately
	return static_cast<uint64_t>(t.tv_sec) * 1000000000llu + static_cast<uint64_t>(t.tv_usec) * 1000llu;
#else
	timespec t;
	if (0 != clock_gettime(CLOCK_MONOTONIC, &t)) {
		return 0;
	}

	return t.tv_sec * 1000000000llu + t.tv_nsec;
#endif
}

class Clock
{
public:
	uint64_t time() const { return _time; }
	void setTime(uint64_t time) { _time = time; }
	void setEpochTime() { _time = epochTime(); }

protected:
	uint64_t _time = 0;
};

}
