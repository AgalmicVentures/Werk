
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

	//TODO: consider an implementation that is less "laggy" where the new value is assumed to be the value for
	//the entire time since the last sample
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
