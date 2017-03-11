
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace werk
{

/**
 * Holds a set of samples and allows taking order statistics on them.
 */
template <typename T, bool handleNaN=false>
class OrderStatistics
{
public:

	uint64_t count() const { return _samples.size(); }

	T min() const {
		if (count() == 0) {
			return 0;
		} else {
			update();
			return _samples[0];
		}
	}
	T max() const {
		if (count() == 0) {
			return 0;
		} else {
			update();
			return _samples[count() - 1];
		}
	}
	T fractile(double f) const {
		if (count() == 0) {
			return 0;
		} else {
			update();
			return _samples[static_cast<size_t>(f * static_cast<double>(count()))];
		}
	}
	T q1() const { return fractile(0.25); }
	T median() const { return fractile(0.5); }
	T q3() const { return fractile(0.75); }

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
