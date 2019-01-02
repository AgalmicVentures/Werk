
/*
 * Copyright (c) 2015-2019 Agalmic Ventures LLC (www.agalmicventures.com)
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

//Work around GCC's lack of a __has_attribute macro
#if defined(__GNUC__) && !defined(__has_attribute)
	#define __has_attribute(X) 1
#endif

#if __has_attribute(always_inline)
	#define ALWAYS_INLINE __attribute__((always_inline))
#else
	#define ALWAYS_INLINE
#endif

#if __has_attribute(warn_unused_result)
	#define CHECKED __attribute__((warn_unused_result))
#else
	#define CHECKED
#endif

#if __has_attribute(cold)
	#define COLD __attribute__((cold))
#else
	#define COLD
#endif

#if __has_attribute(deprecated)
	#define DEPRECATED __attribute__((deprecated))
#else
	#define DEPRECATED
#endif

#if __has_attribute(hot)
	#define HOT __attribute__((hot))
#else
	#define HOT
#endif

#if __has_attribute(pure)
	#define PURE __attribute__((pure))
#else
	#define PURE
#endif

#if __has_attribute(unused)
	#define UNUSED __attribute__((unused))
#else
	#define UNUSED
#endif
