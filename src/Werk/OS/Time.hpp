
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
	return t.tv_sec * 1000000000llu + t.tv_usec * 1000llu;
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
