
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

#include <cinttypes>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <type_traits>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Takes basic summary statistics on a set of samples.
 */
template <typename T>
class SummaryStatistics
{
public:

	CHECKED uint64_t count() const { return _count; }
	CHECKED T sum() const { return _sum; }
	CHECKED double average() const { return _average; }
	CHECKED double m2() const { return _m2; }

	CHECKED double variance() const {
		return _count >= 2 ? _m2 / static_cast<double>(_count) : 0.0;
	}
	CHECKED double stddev() const {
		return _count >= 2 ? std::sqrt(_m2 / static_cast<double>(_count)) : 0.0;
	}

	void sample(T t) {
		_count += 1;
		_sum += t;

		const double t_double = static_cast<double>(t);
		const double delta = t_double - _average;
		_average += delta / static_cast<double>(_count);
		_m2 += delta * (t_double - _average);
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
		const int precision = std::is_floating_point<T>() ? 12 : 0;
		std::fprintf(file, "{\"count\": %" PRIu64 ", \"min\": %.*f, \"max\": %.*f, \"average\": %.12f, \"stddev\": %.12f}",
			this->count(),
			precision, static_cast<double>(_min),
			precision, static_cast<double>(_max),
			this->average(), this->stddev()
		);
	}

private:
	T _min = 0;
	T _max = 0;
};

}
