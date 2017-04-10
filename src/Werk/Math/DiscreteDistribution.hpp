
#pragma once

#include <cstdint>
#include <map>

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
	uint64_t sampleCount() const { return _sampleCount; }
	uint64_t weightCount() const { return _weights.size(); }
	double weightSum() const { return _weightSum; }
	const std::map<T, double> &weights() const { return _weights; }

	//Weight accessors
	double weight(const T &t) const {
		auto i = _weights.find(t);
		return i != _weights.end() ? i->second : 0.0;
	}
	double cumulativeWeight(const T &t) const {
		double sum = 0.0;
		for (auto i = _weights.begin(); i != _weights.end(); ++i)
		{
			if (i->first > t) {
				break;
			}
			sum += i->second;
		}

		return sum;
	}

	//Probability accessors
	double pdf(const T &t) const {
		auto i = _weights.find(t);
		return i != _weights.end() ? i->second / _weightSum : 0.0;
	}
	double cdf(const T &t) const {
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
