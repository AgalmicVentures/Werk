
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include "Attributes.hpp"

/**
 * Generates a bit mask with the first n bits set.
 */
template <typename T>
ALWAYS_INLINE T bitmask(size_t n)
{
	return static_cast<T>((1ul << n) - 1);
}

/**
 * Count the number of 1 bits in a 64-bit number.
 */
static ALWAYS_INLINE
size_t popCount64(uint64_t value)
{
	return (size_t) __builtin_popcountll(value);
}

/**
 * Rounds a value up to the nearest power of two. On overflow, return 0.
 */
template <typename T>
ALWAYS_INLINE T roundUpPow2(T value)
{
	T v = value;
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	if (sizeof(T) > 1) {
		v |= v >> 8;
	}
	if (sizeof(T) > 2) {
		v |= v >> 16;
	}
	if (sizeof(T) > 4) {
		v |= v >> 32;
	}
	++v;
	return v;
}
