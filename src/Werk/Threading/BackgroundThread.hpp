
#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "Werk/OS/Time.hpp"
#include "Werk/Utility/Action.hpp"

namespace werk
{

/**
 * A background thread to run a number of `Action`s at regular intervals.
 *
 * This is very useful e.g. for defering IO to another thread to keep latency low on
 * a main thread.
 */
class BackgroundThread
{
public:

	BackgroundThread(uint64_t frequencyNs=100ul * 1000 * 1000) : _frequencyNs(frequencyNs) {
		_thread = std::thread(&BackgroundThread::backgroundThread, this);
	}
	~BackgroundThread() { stop(); }

	bool stopped() const { return _stopped; }

	uint64_t frequencyNs() const { return _frequencyNs; }
	void setFrequencyNs(uint64_t frequencyNs) { _frequencyNs = frequencyNs; }

	//Tasks in the order they should be executed
	std::vector<Action *> &tasks() { return _tasks; }
	const std::vector<Action *> &tasks() const { return _tasks; }
	void addTask(Action *task) { _tasks.push_back(task); }

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
	std::vector<Action *> _tasks;

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
