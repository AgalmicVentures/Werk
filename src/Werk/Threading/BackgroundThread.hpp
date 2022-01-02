
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <atomic>
#include <thread>
#include <time.h>
#include <vector>

#include "Werk/Logging/Loggable.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Utility/Action.hpp"
#include "Werk/Utility/Attributes.hpp"
#include "Werk/Utility/Latch.hpp"

namespace werk
{

class BackgroundThread;

/**
 * Not for public use.
 */
class BackgroundTask
{
public:

	BackgroundTask(Action *action) :
		_action(action), _profile(std::string("Background_") + action->name()) { }

	CHECKED const Action *action() const { return _action; }
	CHECKED Profile &profile() { return _profile; }
	CHECKED const Profile &profile() const { return _profile; }

	CHECKED Latch<std::atomic<bool> > &active() { return _active; }
	CHECKED const Latch<std::atomic<bool> > &active() const { return _active; }

	void execute();

private:
	Action *_action;
	Profile _profile;
	Latch<std::atomic<bool> > _active { true };
};

/**
 * A background thread to run a number of `Action`s at regular intervals.
 *
 * This is very useful e.g. for defering IO to another thread to keep latency low on
 * a main thread.
 */
class BackgroundThread : public Loggable
{
public:

	BackgroundThread(ProfileManager *profileManager, uint64_t intervalNs=10ul * 1000 * 1000) :
		_profileManager(profileManager), _intervalNs(intervalNs),
		_running(true), _mainClockTime(0)
	{
		_thread = std::thread(&BackgroundThread::backgroundThread, this);
	}
	virtual ~BackgroundThread() { stop(); }

	CHECKED bool stopped() const { return _stopped; }

	CHECKED uint64_t intervalNs() const { return _intervalNs; }
	void setIntervalNs(uint64_t intervalNs) { _intervalNs = intervalNs; }

	//Used to pass time back from the main thread during e.g. simulations
	void setMainClockTime(uint64_t time) { _mainClockTime = time; }

	void logTo(Log *log) const override;

	//Tasks in the order they should be executed
	CHECKED std::vector<BackgroundTask *> &tasks() { return _tasks; }
	CHECKED const std::vector<BackgroundTask *> &tasks() const { return _tasks; }
	void addTask(Action *action) {
		BackgroundTask *task = new BackgroundTask(action);
		if (nullptr != _profileManager) {
			_profileManager->add(&task->profile());
		}
		_tasks.push_back(task);
	}

	CHECKED const Clock &mainClock() const { return _backgroundClock; }
	CHECKED const Clock &backgroundClock() const { return _backgroundClock; }

	void stop() {
		if (_running) {
			_running = false;
			_thread.join();
		}
		_stopped = true;
	}

private:
	//Config before
	ProfileManager *_profileManager;
	std::vector<BackgroundTask *> _tasks;

	//Background thread
	std::thread _thread;
	bool _stopped = false;

	//Shared state
	std::atomic<uint64_t> _intervalNs;
	std::atomic<bool> _running;
	std::atomic<uint64_t> _mainClockTime;

	//Background thread state & method
	Clock _mainClock; //NOTE: this clock is not the actual main thread clock, only synchronized to it
	Clock _backgroundClock;
	timespec _delay;
	void backgroundThread();
};

}
