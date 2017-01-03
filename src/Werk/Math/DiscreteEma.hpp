
#pragma once

#include <cmath>
#include <limits>

namespace werk
{

//Implements an EMA that goes in discrete steps
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
