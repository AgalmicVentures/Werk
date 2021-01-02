
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
 * Implements an EMA that is continuous e.g. in time.
 */
class ContinuousEma
{
public:

	ContinuousEma(double halfLife) {
		setHalfLife(halfLife);
		reset();
	}

	double factor() const { return _factor; }
	double halfLife() const { return _halfLife; }
	double value() const { return _value; }
	double lastIndex() const { return _lastIndex; }

	void setHalfLife(double halfLife) {
		assert(halfLife >= 0.0);
		_halfLife = halfLife;
		_factor = std::log(2.0) / halfLife;
		assert(_factor >= 0.0);
	}

	//Modifiers for the value, which allow interesting use cases such as exponentially decaying sums
	//(by using the add() method and then sample(0.0) or whatever the set point is).
	void add(double delta) { _value += delta; }
	void setValue(double value) { _value = value; }

	void sample(double index, double sample) {
		if (std::isnan(_value)) {
			_value = sample;
		} else {
			//Calculating 1 - alpha since after 3 half lives, 1/8 should be left of the old value and 7/8 of the value should be the new
			//Intentionally remove the negation by swapping the subtraction
			const double logOneMinusAlpha = _factor * (_lastIndex - index);
			const double oneMinusAlpha = std::exp(logOneMinusAlpha);
			_value = (1.0 - oneMinusAlpha) * sample + oneMinusAlpha * _value;
		}
		_lastIndex = index;
	}

	void reset() {
		_lastIndex = _value = std::numeric_limits<double>::quiet_NaN();
	}

private:
	double _halfLife;
	double _factor;

	double _value = std::numeric_limits<double>::quiet_NaN();
	double _lastIndex = std::numeric_limits<double>::quiet_NaN();
};

}
