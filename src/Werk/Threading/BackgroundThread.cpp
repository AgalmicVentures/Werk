
#include "BackgroundThread.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>

#include "Werk/OS/Time.hpp"

namespace werk
{

/**
 * This method is the actual background thread.
 *
 * Unfortunately neither while nor do..while allows us to avoid the break in
 * the middle if the goal is to minimize latency.
 */
void BackgroundThread::backgroundThread()
{
	while (true) {
		//Execute all the tasks
		for (size_t i = 0; i < _tasks.size(); ++i) {
			_tasks[i]->executeTask();
		}

		//Break here to ensure that the last few lines of logs get written
		if (!_running) {
			break;
		}

		//Delay, updating the frequency since it may be updated on another thread
		const uint64_t nanosPerSecond = 1000000000l;
		_delay.tv_sec = _frequencyNs / nanosPerSecond;
		_delay.tv_nsec = _frequencyNs % nanosPerSecond;
		nanosleep(&_delay, NULL);
	}
}

}
