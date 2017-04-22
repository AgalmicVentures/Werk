
#include "BackgroundThread.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cstdarg>
#include <cstdio>

#include "Werk/Logging/Log.hpp"
#include "Werk/OS/Time.hpp"

namespace werk
{

void BackgroundTask::execute()
{
	if (!_active.value()) {
		return;
	}

	PROFILE_START(_profile);
	_action->execute();
	PROFILE_STOP(_profile);
}

void BackgroundThread::logTo(Log *log) const
{
	log->log(LogLevel::INFO, "<BackgroundThread> Frequency (ns): %" PRIu64, _intervalNs);
	log->log(LogLevel::INFO, "<BackgroundThread> Tasks (%zu):", _tasks.size());
	for (BackgroundTask *task : _tasks) {
		log->log(LogLevel::INFO, "  %24s    %8s    Count=%" PRIu64,
			task->action()->name().c_str(), task->active().value() ? "Active" : "Inactive", task->profile().count());
	}
}

/**
 * This method is the actual background thread.
 *
 * Unfortunately neither while nor do..while allows us to avoid the break in
 * the middle if the goal is to minimize latency.
 */
void BackgroundThread::backgroundThread()
{
	while (true) {
		//Update the time
		_mainClock.setTime(_mainClockTime);
		_backgroundClock.setEpochTime();

		//Execute all the tasks
		for (size_t i = 0; i < _tasks.size(); ++i) {
			_tasks[i]->execute();
		}

		//Break here to ensure that the last few lines of logs get written
		if (!_running) {
			break;
		}

		//Delay, updating the interval since it may be updated on another thread
		const uint64_t nanosPerSecond = 1000000000l;
		_delay.tv_sec = _intervalNs / nanosPerSecond;
		_delay.tv_nsec = _intervalNs % nanosPerSecond;
		nanosleep(&_delay, nullptr);
	}
}

}
