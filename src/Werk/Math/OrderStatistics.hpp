
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Holds a set of samples and allows taking order statistics on them.
 */
template <typename T, bool handleNaN=false>
class OrderStatistics
{
public:

	CHECKED uint64_t count() const { return _samples.size(); }

	CHECKED T min() const {
		if (count() == 0) {
			return 0;
		} else {
			update();
			return _samples[0];
		}
	}
	CHECKED T max() const {
		if (count() == 0) {
			return 0;
		} else {
			update();
			return _samples[count() - 1];
		}
	}
	CHECKED T fractile(double f) const {
		assert(0.0 <= f);
		assert(f <= 1.0);
		if (count() == 0) {
			return 0;
		} else {
			update();
			return _samples[static_cast<size_t>(f * static_cast<double>(count()))];
		}
	}
	CHECKED T q1() const { return fractile(0.25); }
	CHECKED T median() const { return fractile(0.5); }
	CHECKED T q3() const { return fractile(0.75); }

	void sample(T t) {
		if (handleNaN && std::isnan(t)) {
			return;
		}

		_samples.push_back(t);
		_dirty = true;
	}
	void reset() {
		_samples.clear();
		_dirty = false;
	}

private:

	inline void update() const {
		if (_dirty) {
			std::sort(_samples.begin(), _samples.end());
			_dirty = false;
		}
	}

	mutable std::vector<T> _samples;
	mutable bool _dirty = false;
};

}
