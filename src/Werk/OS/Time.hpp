
#ifndef __MACH__
#include <time.h>
#else
#include <sys/time.h>
#endif

namespace werk {

uint64_t epochNs()
{
#ifndef __MACH__
	timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) < 0)
	{
		return 0;
	}

	return t.tv_sec * 1e9l + t.tv_nsec;
#else
	//The Mac version only has microsecond resolution
	struct timeval now;
	if (gettimeofday(&now, nullptr) != 0)
	{
		return 0;
	}

	return now.tv_sec * 1e9l + now.tv_usec * 1e3l;
#endif
}

class Clock {

public:

	uint64_t time() const { return _time; }
	void setTime(uint64_t time) { _time = time; }

	void setRealTime() { _time = epochNs(); }

protected:
	uint64_t _time = 0;
};

}
