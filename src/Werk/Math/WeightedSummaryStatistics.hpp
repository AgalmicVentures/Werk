
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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
		_sum += value * weight;

		const double delta = value - _average;
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
		std::fprintf(file, "{\"count\": %" PRIu64 ", \"average\": %.12f, \"stddev\": %.12f}", _count, _average, stddev());
	}

private:
	uint64_t _count = 0;
	double _weightSum = 0;
	double _sum = 0;
	double _average = 0;
	double _m2 = 0;
};

}
