
#pragma once

#include <cstddef>
#include <cstdint>

namespace werk
{

/**
 * Returns the size of a page in bytes.
 */
uint64_t getPageSize();

/**
 * Returns the size of the physical memory in bytes.
 */
uint64_t getPhysicalMemorySize();

/**
 * Returns the number of processors on this system.
 */
size_t getProcessorCount();

}
