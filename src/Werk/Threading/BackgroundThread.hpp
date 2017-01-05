
#pragma once

#include <thread>
#include <time.h>
#include <vector>

#include "BackgroundTask.hpp"

namespace werk
{

class BackgroundThread
{
public:

	BackgroundThread(long frequencyNs=100ul * 1000 * 1000) : _frequencyNs(frequencyNs) {
		_thread = std::thread(&BackgroundThread::backgroundThread, this);
	}
	~BackgroundThread() { stop(); }

	uint64_t frequencyNs() const { return _frequencyNs; }
	void setFrequencyNs(uint64_t frequencyNs) { _frequencyNs = frequencyNs; }

	std::vector<BackgroundTask *> &tasks() { return _tasks; }
	const std::vector<BackgroundTask *> &tasks() const { return _tasks; }
	void addTask(BackgroundTask *task) { _tasks.push_back(task); }

	void stop() {
		if (_running) {
			_running = false;
			_thread.join();
		}
	}

private:
	timespec _delay;
	std::vector<BackgroundTask *> _tasks;

	volatile uint64_t _frequencyNs;
	volatile bool _running = true;
	std::thread _thread;

	void backgroundThread();
};

}
