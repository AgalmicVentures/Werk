
#pragma once

#include <cmath>
#include <cstdint>
#include <limits>

#include "SummaryStatistics.hpp"

namespace werk
{

/**
 * Calculates a simple linear regression between two sets of samples.
 */
class SimpleLinearRegression
{
public:

	uint64_t count() const { return _x.count(); }

	double covariance() const { return _xy.average() - _x.average() * _y.average(); }
	double correlation() const {
		return count() >= 2 ?
			std::max(-1.0, std::min(1.0, covariance() / std::sqrt(_x.variance() * _y.variance()))) :
			std::numeric_limits<double>::quiet_NaN();
	}
	double beta() const {
		return count() >= 2 ? covariance() / _x.variance() : std::numeric_limits<double>::quiet_NaN();
	}
	double alpha() const {
		return _y.average() - _x.average() * beta();
	}

	//Predicts a y value given an x value
	double predict(double x) {
		return beta() * (x - _x.average()) + _y.average();
	}

	void sample(double x, double y) {
		_x.sample(x);
		_y.sample(y);
		_xy.sample(x * y);
	}
	void reset() {
		_x.reset();
		_y.reset();
		_xy.reset();
	}

private:
	SummaryStatistics<double> _x;
	SummaryStatistics<double> _y;
	SummaryStatistics<double> _xy;
};

}
