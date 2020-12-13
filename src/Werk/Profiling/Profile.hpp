
/*
 * Copyright (c) 2015-2020 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Math/OrderStatistics.hpp"
#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/Utility/NamedObject.hpp"

#define PROFILE_START(p) (p).start(werk::monotoneTime())
#define PROFILE_STOP(p) (p).stop(werk::monotoneTime())

namespace werk
{

/**
 * Holds information about the timing profile of a single section of code/other event.
 */
class Profile : public NamedObject
{
public:

	Profile(const std::string &name, uint64_t sampleSize = 100, uint64_t warmupSize = 0)
		: NamedObject(name), _sampleSize(sampleSize), _warmupSize(warmupSize) { }

	uint64_t sampleSize() const { return _sampleSize; }
	uint64_t count() const { return _count; }
	const RangedSummaryStatistics<uint64_t> &warmupStatistics() const { return _warmupStatistics; }
	const OrderStatistics<uint64_t> &orderStatistics() const { return _orderStatistics; }
	const RangedSummaryStatistics<uint64_t> &minStatistics() const { return _minStatistics; }
	const RangedSummaryStatistics<uint64_t> &f25Statistics() const { return _f25Statistics; }
	const RangedSummaryStatistics<uint64_t> &f50Statistics() const { return _f50Statistics; }
	const RangedSummaryStatistics<uint64_t> &f75Statistics() const { return _f75Statistics; }
	const RangedSummaryStatistics<uint64_t> &f90Statistics() const { return _f90Statistics; }
	const RangedSummaryStatistics<uint64_t> &f95Statistics() const { return _f95Statistics; }
	const RangedSummaryStatistics<uint64_t> &f99Statistics() const { return _f99Statistics; }
	const RangedSummaryStatistics<uint64_t> &maxStatistics() const { return _maxStatistics; }

	void start(uint64_t time) { _startTime = time; }
	void stop(uint64_t time) {
		//Started?
		if (_startTime == 0) {
			return;
		}

		//Warmup?
		const uint64_t delta = time - _startTime;
		_count += 1;
		if (_count <= _warmupSize) {
			_warmupStatistics.sample(delta);
			return;
		}

		//Sample
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

		_minStatistics.sample(_orderStatistics.min());
		_f25Statistics.sample(_orderStatistics.fractile(0.25));
		_f50Statistics.sample(_orderStatistics.fractile(0.50));
		_f75Statistics.sample(_orderStatistics.fractile(0.75));
		_f90Statistics.sample(_orderStatistics.fractile(0.90));
		_f95Statistics.sample(_orderStatistics.fractile(0.95));
		_f99Statistics.sample(_orderStatistics.fractile(0.99));
		_maxStatistics.sample(_orderStatistics.max());
		_orderStatistics.reset();
	}
	void reset() {
		_count = 0;
		_startTime = 0;
		_orderStatistics.reset();
		_warmupStatistics.reset();
		_minStatistics.reset();
		_f25Statistics.reset();
		_f50Statistics.reset();
		_f75Statistics.reset();
		_f90Statistics.reset();
		_f95Statistics.reset();
		_f99Statistics.reset();
		_maxStatistics.reset();
	}

	void writeJson(FILE *file) const {
		std::fprintf(file, "{\"name\": \"%s\"", name().c_str());
		std::fprintf(file, ", \"n\": %" PRIu64, _count);
		std::fprintf(file, ", \"sampleSize\": %" PRIu64, _sampleSize);
		std::fprintf(file, ", \"warmup\": "); _warmupStatistics.writeJson(file);
		std::fprintf(file, ", \"min\": "); _minStatistics.writeJson(file);
		std::fprintf(file, ", \"f25\": "); _f25Statistics.writeJson(file);
		std::fprintf(file, ", \"f50\": "); _f50Statistics.writeJson(file);
		std::fprintf(file, ", \"f75\": "); _f75Statistics.writeJson(file);
		std::fprintf(file, ", \"f90\": "); _f90Statistics.writeJson(file);
		std::fprintf(file, ", \"f95\": "); _f95Statistics.writeJson(file);
		std::fprintf(file, ", \"f99\": "); _f99Statistics.writeJson(file);
		std::fprintf(file, ", \"max\": "); _maxStatistics.writeJson(file);
		std::fprintf(file, "}\n");
	}

private:

	//Configuration
	uint64_t _sampleSize;
	uint64_t _warmupSize;

	//State - values bubble downward

	//Current state
	uint64_t _count = 0;
	uint64_t _startTime = 0;

	//Samples used for approximating fractiles efficiently
	OrderStatistics<uint64_t> _orderStatistics;

	//Statistics taken from samples of the order statstics every _sampleSize samples (or the warmup)
	RangedSummaryStatistics<uint64_t> _warmupStatistics;
	RangedSummaryStatistics<uint64_t> _minStatistics;
	RangedSummaryStatistics<uint64_t> _f25Statistics;
	RangedSummaryStatistics<uint64_t> _f50Statistics;
	RangedSummaryStatistics<uint64_t> _f75Statistics;
	RangedSummaryStatistics<uint64_t> _f90Statistics;
	RangedSummaryStatistics<uint64_t> _f95Statistics;
	RangedSummaryStatistics<uint64_t> _f99Statistics;
	RangedSummaryStatistics<uint64_t> _maxStatistics;
};

}
