
#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Utility/Action.hpp"

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

	Profile &profile() { return _profile; }
	const Profile &profile() const { return _profile; }

	void execute();

private:
	Action *_action;
	Profile _profile;
};

/**
 * A background thread to run a number of `Action`s at regular intervals.
 *
 * This is very useful e.g. for defering IO to another thread to keep latency low on
 * a main thread.
 */
class BackgroundThread
{
public:

	BackgroundThread(ProfileManager *profileManager, uint64_t frequencyNs=10ul * 1000 * 1000) :
		 _profileManager(profileManager), _frequencyNs(frequencyNs)
	{
		_thread = std::thread(&BackgroundThread::backgroundThread, this);
	}
	~BackgroundThread() { stop(); }

	bool stopped() const { return _stopped; }

	uint64_t frequencyNs() const { return _frequencyNs; }
	void setFrequencyNs(uint64_t frequencyNs) { _frequencyNs = frequencyNs; }

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
	volatile uint64_t _frequencyNs;
	volatile bool _running = true;

	//Background thread state & method
	Clock _backgroundClock;
	timespec _delay;
	void backgroundThread();
};

}
