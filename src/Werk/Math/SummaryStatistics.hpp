
#pragma once

#include <cmath>
#include <cstdint>
#include <limits>

namespace werk
{

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
		return _count >= 2 ? std::sqrt(variance()) : 0.0;
	}

	void sample(T t) {
		_count += 1;
		_sum += t;

		double delta = t - _average;
		_average += delta / static_cast<double>(_count);
		_m2 += delta * (t - _average);
	}

private:
	uint64_t _count = 0;
	T _sum = 0;
	double _average = 0;
	double _m2 = 0;
};

}
