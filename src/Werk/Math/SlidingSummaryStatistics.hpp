
#pragma once

#include "Werk/Math/WeightedSummaryStatistics.hpp"
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

	const SummaryStatistics<TValue> &statistics() const { return _statistics; }

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
