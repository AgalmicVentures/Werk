
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

#include <cstdint>
#include <map>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Represents a distribution over a discrete set of values; that is, each value
 * has a weight, and by dividing by the sum of the weights, a probability.
 *
 * Note that sampling with negative weights may break some accessors.
 */
template <typename T>
class DiscreteDistribution
{
public:
	//Basic accessors
	CHECKED uint64_t sampleCount() const { return _sampleCount; }
	CHECKED uint64_t weightCount() const { return _weights.size(); }
	CHECKED double weightSum() const { return _weightSum; }
	CHECKED const std::map<T, double> &weights() const { return _weights; }

	//Weight accessors
	CHECKED double weight(const T &t) const {
		auto i = _weights.find(t);
		return i != _weights.end() ? i->second : 0.0;
	}
	CHECKED double cumulativeWeight(const T &t) const {
		double sum = 0.0;
		for (const auto &i : _weights) {
			if (i.first > t) {
				break;
			}
			sum += i.second;
		}

		return sum;
	}

	//Probability accessors
	CHECKED double pdf(const T &t) const {
		auto i = _weights.find(t);
		return i != _weights.end() ? i->second / _weightSum : 0.0;
	}
	CHECKED double cdf(const T &t) const {
		return cumulativeWeight(t) / _weightSum;
	}

	//Sampling methods
	void sample(const T &t, double dWeight) {
		_sampleCount += 1;
		_weightSum += dWeight;
		_weights[t] += dWeight;
	}
	void reset() {
		_sampleCount = 0;
		_weightSum = 0.0;
		_weights.clear();
	}

private:
	uint64_t _sampleCount = 0;
	double _weightSum = 0.0;
	std::map<T, double> _weights;
};

}
