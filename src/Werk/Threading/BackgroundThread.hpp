
#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "Werk/Logging/Loggable.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Utility/Action.hpp"
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

	const Action *action() const { return _action; }
	Profile &profile() { return _profile; }
	const Profile &profile() const { return _profile; }

	Latch<volatile bool> &active() { return _active; }
	const Latch<volatile bool> &active() const { return _active; }

	void execute();

private:
	Action *_action;
	Profile _profile;
	Latch<volatile bool> _active { true };
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
		 _profileManager(profileManager), _intervalNs(intervalNs)
	{
		_thread = std::thread(&BackgroundThread::backgroundThread, this);
	}
	~BackgroundThread() { stop(); }

	bool stopped() const { return _stopped; }

	uint64_t intervalNs() const { return _intervalNs; }
	void setIntervalNs(uint64_t intervalNs) { _intervalNs = intervalNs; }

	//Used to pass time back from the main thread during e.g. simulations
	void setMainClockTime(uint64_t time) { _mainClockTime = time; }

	void logTo(Log *log) const override;

	//Tasks in the order they should be executed
	std::vector<BackgroundTask *> &tasks() { return _tasks; }
	const std::vector<BackgroundTask *> &tasks() const { return _tasks; }
	void addTask(Action *action) {
		BackgroundTask *task = new BackgroundTask(action);
		if (nullptr != _profileManager) {
			_profileManager->add(&task->profile());
		}
		_tasks.push_back(task);
	}

	const Clock &mainClock() const { return _backgroundClock; }
	const Clock &backgroundClock() const { return _backgroundClock; }

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
	volatile uint64_t _intervalNs;
	volatile bool _running = true;
	volatile uint64_t _mainClockTime = 0;

	//Background thread state & method
	Clock _mainClock; //NOTE: this clock is not the actual main thread clock, only synchronized to it
	Clock _backgroundClock;
	timespec _delay;
	void backgroundThread();
};

}
