
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Werk/Data/TimeSeries.hpp"
#include "Werk/Math/Random.hpp"
#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Represents a time series iterator.
 */
class JitterTimeSeries : public TimeSeries
{
public:

	JitterTimeSeries(Random &random, TimeSeries *underlying, uint64_t min, uint64_t max)
		: _random(random), _underlying(underlying), _min(min), _max(max) { }

	//Return the current time plus some jitter.
	CHECKED virtual uint64_t time() const { return _time; }

	//Moves to the next time, updating the jitter as it goes.
	CHECKED virtual bool moveNext() {
		if (_underlying->moveNext()) {
			uint64_t jitter = _random.getUint64(_min, _max);

			//Only allow time to move forward
			_time = std::max(_time, _underlying->time() + jitter);
			return true;
		} else {
			return false;
		}
	}

protected:

	Random &_random;
	TimeSeries *_underlying;
	uint64_t _min;
	uint64_t _max;

	//The time of the current event, adjusted by jitter.
	//This is stored as state, rather than just the jitter, in order to ensure
	//that the times generated are monotonic.
	uint64_t _time = 0;

};

}
