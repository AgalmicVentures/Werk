
#pragma once

#include <algorithm>
#include <cstdint>
#include <limits>
#include <vector>

namespace werk
{

template <typename T>
class OrderStatistics
{
public:

	uint64_t count() const { return _samples.size(); }

	double min() const {
		if (count() == 0) {
			return std::numeric_limits<double>::quiet_NaN();
		} else {
			update();
			return _samples[0];
		}
	}
	double max() const {
		if (count() == 0) {
			return std::numeric_limits<double>::quiet_NaN();
		} else {
			update();
			return _samples[count() - 1];
		}
	}
	double fractile(double f) const {
		if (count() == 0) {
			return std::numeric_limits<double>::quiet_NaN();
		} else {
			update();
			return _samples[static_cast<size_t>(f * static_cast<double>(count()))];
		}
	}
	double q1() const { return fractile(0.25); }
	double median() const { return fractile(0.5); }
	double q3() const { return fractile(0.75); }

	void sample(T t) {
		_samples.push_back(t);
		_dirty = true;
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
