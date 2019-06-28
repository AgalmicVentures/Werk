
/*
 * Copyright (c) 2015-2019 Agalmic Ventures LLC (www.agalmicventures.com)
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
	log->log(LogLevel::INFO, "<BackgroundThread> Frequency (ns): %" PRIu64, _intervalNs.load());
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
