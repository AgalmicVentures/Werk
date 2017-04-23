
#pragma once

#include <string>
#include <vector>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

typedef std::vector<std::pair<std::string, uint64_t> > UnitsT;

extern const UnitsT STORAGE_UNITS;
extern const UnitsT TIME_UNITS;

/**
 * Calculates the information needed to format the value with units:
 * the scaled value, and the units themselves.
 */
void formatUnits(uint64_t value, const UnitsT &units, double &scaledValue, const char *&unitStr);

/**
 * Parses a value with the given sort of units.
 */
CHECKED PURE uint64_t parseUnits(const std::string &value, const UnitsT &units);

}
