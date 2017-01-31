
#pragma once

#include <string>
#include <vector>

namespace werk
{

typedef std::vector<std::pair<std::string, uint64_t> > UnitsT;

extern const UnitsT STORAGE_UNITS;
extern const UnitsT TIME_UNITS;

/**
 * Parses a value with the given sort of units.
 */
uint64_t parseUnits(const std::string &value, const UnitsT &units);

}
