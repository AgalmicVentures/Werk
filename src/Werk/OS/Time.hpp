
#pragma once

#ifndef __MACH__
#include <time.h>
#else
#include <sys/time.h>
#endif

namespace werk {

inline uint64_t epochTime()
{
#ifndef __MACH__
	timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) < 0) {
		return 0;
	}

	return t.tv_sec * 1e9l + t.tv_nsec;
#else
	struct timeval t;
	if (gettimeofday(&t, nullptr) != 0) {
		return 0;
	}

	//Only microsecond resolution, unfortunately
	return t.tv_sec * 1e9l + t.tv_usec * 1e3l;
#endif
}

class Clock {

public:

	uint64_t time() const { return _time; }
	void setTime(uint64_t time) { _time = time; }
	void setEpochTime() { _time = epochTime(); }

protected:
	uint64_t _time = 0;
};

}
