
#pragma once

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

	double average() const {
		return _count > 0 ?
			static_cast<double>(_sum) / static_cast<double>(_count) :
			std::numeric_limits<double>::quiet_NaN();
	}

	void sample(T t) {
		_count += 1;
		_sum += t;
	}

private:
	uint64_t _count = 0;
	T _sum = 0;
};

}
