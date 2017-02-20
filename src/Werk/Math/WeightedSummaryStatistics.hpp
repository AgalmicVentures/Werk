
#pragma once

#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>

namespace werk
{

/**
 * Takes basic summary statistics on a weighted set of samples.
 */
class WeightedSummaryStatistics
{
public:

	uint64_t count() const { return _count; }
	double weightSum() const { return _weightSum; }
	double sum() const { return _sum; }
	double average() const { return _average; }
	double m2() const { return _m2; }

	double variance() const {
		return _weightSum != 0.0 ? _m2 / _weightSum : 0.0;
	}
	double stddev() const {
		return  _weightSum != 0.0 ? std::sqrt(_m2 / _weightSum ) : 0.0;
	}

	void sample(double value, double weight) {
		_count += 1;
		_weightSum += weight;
		_sum += value;

		double delta = value - _average;
		_average += delta * weight / _weightSum;
		_m2 += delta * weight * (value - _average);
	}
	void reset() {
		_count = 0;
		_weightSum = 0;
		_sum = 0;
		_average = 0;
		_m2 = 0;
	}

	void writeJson(FILE *file) const {
		fprintf(file, "{\"count\": %" PRIu64 ", \"average\": %.12f, \"stddev\": %.12f}", _count, _average, stddev());
	}

private:
	uint64_t _count = 0;
	double _weightSum = 0;
	double _sum = 0;
	double _average = 0;
	double _m2 = 0;
};

}
