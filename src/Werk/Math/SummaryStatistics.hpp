
#pragma once

#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>

namespace werk
{

/**
 * Takes basic summary statistics on a set of samples.
 */
template <typename T>
class SummaryStatistics
{
public:

	uint64_t count() const { return _count; }
	T sum() const { return _sum; }
	double average() const { return _average; }
	double m2() const { return _m2; }

	double variance() const {
		return _count >= 2 ? _m2 / static_cast<double>(_count) : 0.0;
	}
	double stddev() const {
		return _count >= 2 ? std::sqrt(_m2 / static_cast<double>(_count)) : 0.0;
	}

	void sample(T t) {
		_count += 1;
		_sum += t;

		double delta = t - _average;
		_average += delta / static_cast<double>(_count);
		_m2 += delta * (t - _average);
	}
	void reset() {
		_count = 0;
		_sum = 0;
		_average = 0;
		_m2 = 0;
	}

	void writeJson(FILE *file) const {
		std::fprintf(file, "{\"count\": %" PRIu64 ", \"average\": %.12f, \"stddev\": %.12f}", _count, _average, stddev());
	}

private:
	uint64_t _count = 0;
	T _sum = 0;
	double _average = 0;
	double _m2 = 0;
};

/**
 * Calculates summary statistics, and min/max.
 */
template <typename T>
class RangedSummaryStatistics : public SummaryStatistics<T>
{
public:

	T min() const { return _min; }
	T max() const { return _max; }
	T range() const { return _max - _min; }

	void sample(T t) {
		SummaryStatistics<T>::sample(t);
		if (this->count() == 1) {
			_min = _max = t;
		} else if (t > _max) {
			_max = t;
		} else if (t < _min) {
			_min = t;
		}
	}

	void writeJson(FILE *file) const {
		std::fprintf(file, "{\"count\": %" PRIu64 ", \"average\": %.12f, \"stddev\": %.12f, \"min\": %.12f, \"max\": %.12f}",
			this->count(), this->average(), this->stddev(), static_cast<double>(_min), static_cast<double>(_max));
	}

private:
	T _min = 0;
	T _max = 0;
};

}
