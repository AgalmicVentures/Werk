
#pragma once

#include <cinttypes>

namespace werk
{

/**
 * Represents a time series iterator.
 */
class TimeSeries
{
public:

	/**
	 * Return the current time.
	 */
	virtual uint64_t time() = 0;

	/**
	 * Moves to the next time, returning false when there is no more data available.
	 */
	virtual bool moveNext() = 0;
};

}
