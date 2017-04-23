
#pragma once

#include <cstddef>
#include <cstdint>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

/**
 * Returns the size of a page in bytes.
 */
CHECKED uint64_t getPageSize();

/**
 * Returns the size of the physical memory in bytes.
 */
CHECKED uint64_t getPhysicalMemorySize();

/**
 * Returns the number of processors on this system.
 */
CHECKED size_t getProcessorCount();

}
