
#pragma once

#include "Werk/Math/OrderStatistics.hpp"
#include "Werk/Math/SummaryStatistics.hpp"

namespace werk
{

class Profile
{
public:

	Profile(uint64_t sampleSize = 100) : _sampleSize(sampleSize) { }

	uint64_t sampleSize() const { return _sampleSize; }
	const OrderStatistics<uint64_t> &orderStatistics() const { return _orderStatistics; }
	const SummaryStatistics<uint64_t> &f50Statistics() const { return _f50Statistics; }
	const SummaryStatistics<uint64_t> &f75Statistics() const { return _f75Statistics; }
	const SummaryStatistics<uint64_t> &f90Statistics() const { return _f90Statistics; }
	const SummaryStatistics<uint64_t> &f95Statistics() const { return _f95Statistics; }
	const SummaryStatistics<uint64_t> &f99Statistics() const { return _f99Statistics; }

	void start(uint64_t time) {
		_startTime = time;
	}
	void stop(uint64_t time) {
		//Started?
		if (_startTime == 0) {
			return;
		}

		//Sample
		uint64_t delta = time - _startTime;
		_orderStatistics.sample(delta);
		_startTime = 0;

		//Bin done?
		if (_orderStatistics.count() >= _sampleSize) {
			sampleFractiles();
		}
	}
	void restart(uint64_t time) {
		stop(time);
		start(time);
	}

	void sampleFractiles() {
		if (_orderStatistics.count() == 0) {
			return;
		}

		_f50Statistics.sample(_orderStatistics.fractile(0.50));
		_f75Statistics.sample(_orderStatistics.fractile(0.75));
		_f90Statistics.sample(_orderStatistics.fractile(0.90));
		_f95Statistics.sample(_orderStatistics.fractile(0.95));
		_f99Statistics.sample(_orderStatistics.fractile(0.99));
		_orderStatistics.reset();
	}
	void reset() {
		_startTime = 0;
		_orderStatistics.reset();
		_f50Statistics.reset();
		_f75Statistics.reset();
		_f90Statistics.reset();
		_f95Statistics.reset();
		_f99Statistics.reset();
	}

private:

	//Current state
	uint64_t _startTime = 0;

	//Samples used for approximating fractiles efficiently
	uint64_t _sampleSize;
	OrderStatistics<uint64_t> _orderStatistics;

	//Statistics taken from samples of the order statstics every _sampleSize samples
	SummaryStatistics<uint64_t> _f50Statistics;
	SummaryStatistics<uint64_t> _f75Statistics;
	SummaryStatistics<uint64_t> _f90Statistics;
	SummaryStatistics<uint64_t> _f95Statistics;
	SummaryStatistics<uint64_t> _f99Statistics;
};

}
