
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

#include <cmath>
#include <cstdint>
#include <limits>

#include "SummaryStatistics.hpp"

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Calculates a simple linear regression between two sets of samples.
 */
class SimpleLinearRegression
{
public:

	CHECKED uint64_t count() const { return _x.count(); }

	CHECKED double covariance() const { return _xy.average() - _x.average() * _y.average(); }
	CHECKED double correlation() const {
		return count() >= 2 ?
			std::max(-1.0, std::min(1.0, covariance() / std::sqrt(_x.variance() * _y.variance()))) :
			std::numeric_limits<double>::quiet_NaN();
	}
	CHECKED double beta() const {
		return count() >= 2 ? covariance() / _x.variance() : std::numeric_limits<double>::quiet_NaN();
	}
	CHECKED double alpha() const {
		return _y.average() - _x.average() * beta();
	}

	//Predicts a y value given an x value
	CHECKED double predict(double x) {
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
