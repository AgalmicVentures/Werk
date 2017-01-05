
#include "BackgroundThread.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>

#include "Werk/OS/Time.hpp"

namespace werk
{

void BackgroundThread::backgroundThread()
{
	while (true) {
		//Execute all the tasks
		for (size_t i = 0; i < _tasks.size(); ++i) {
			_tasks[i]->execute();
		}

		//Break here to ensure that the last few lines of logs get written
		if (!_running) {
			break;
		}

		//Delay, updating the frequency since it may be updated on another thread
		uint64_t nanosPerSecond = 1000000000l;
		_delay.tv_sec = _frequencyNs / nanosPerSecond;
		_delay.tv_nsec = _frequencyNs % nanosPerSecond;
		nanosleep(&_delay, NULL);
	}
}

}
