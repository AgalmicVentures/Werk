
#pragma once

#include <cinttypes>

#include "Werk/Utility/Attributes.hpp"

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
	CHECKED virtual uint64_t time() const = 0;

	/**
	 * Moves to the next time, returning false when there is no more data available.
	 */
	CHECKED virtual bool moveNext() = 0;
};

}
