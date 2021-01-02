
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

#include <cmath>
#include <limits>

namespace werk
{

/**
 * Implements an EMA that goes in discrete steps.
 */
class DiscreteEma
{
public:

	DiscreteEma(double alpha) : _alpha(alpha) { }

	double alpha() const { return _alpha; }
	double value() const { return _value; }

	void sample(double x) {
		_value = std::isnan(_value) ? x : (1.0 - _alpha) * _value + _alpha * x;
	}
	void reset() {
		_value = std::numeric_limits<double>::quiet_NaN();
	}

private:
	//Config
	double _alpha;

	//State
	double _value = std::numeric_limits<double>::quiet_NaN();
};

}
