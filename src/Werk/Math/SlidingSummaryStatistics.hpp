
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

#include "Werk/Math/WeightedSummaryStatistics.hpp"
#include "Werk/Utility/Attributes.hpp"
#include "Werk/Utility/SlidingWindow.hpp"

namespace werk
{

/**
 * A sliding window that calculates summary statistics.
 */
template <typename TIndex, typename TValue, size_t size>
class SlidingSummaryStatistics : public BaseSlidingWindow<TIndex, TValue, size, SlidingWindow<TIndex, TValue, size> >
{
public:
	SlidingSummaryStatistics(TIndex duration) : BaseSlidingWindow<TIndex, TValue, size, SlidingWindow<TIndex, TValue, size> >(duration) { }

	CHECKED const SummaryStatistics<TValue> &statistics() const { return _statistics; }

	void onAddValue(TIndex index, TValue value) {
		(void) index;
		_statistics.sample(value, 1.0);
	}
	void onRemoveValue(TIndex index, TValue value) {
		(void) index;
		_statistics.sample(value, -1.0);
	}

protected:
	WeightedSummaryStatistics<TValue> _statistics;
};

}
