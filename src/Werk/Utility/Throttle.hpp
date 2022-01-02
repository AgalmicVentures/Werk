
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

#include <cassert>
#include <cinttypes>

#include "Attributes.hpp"

namespace werk
{

/**
 * Throttle, similar to a sliding window with no values.
 */
template <typename TIndex, size_t size>
class Throttle
{
public:
	Throttle(TIndex duration) : _duration(duration) { }

	CHECKED TIndex duration() const { return _duration; }

	CHECKED size_t count() const { return _count; }
	CHECKED bool isEmpty() const { return _count == 0; }
	CHECKED bool isFull() const { return _count == size; }

	//NOTE: These are not bounds checked since there is no sensible default
	CHECKED TIndex frontIndex() const { return _indexes[_front]; }

	void slideOne() {
		if (_count == 0) {
			return;
		}

		forceSlideOne();
	}
	void slide(TIndex index) {
		while (_count > 0 && index - _indexes[_front] > _duration) {
			forceSlideOne();
		}
	}
	void sample(TIndex index) {
		//Make room if necessary
		if (isFull()) {
			forceSlideOne();
		}

		const size_t back = backLocation();
		_indexes[back] = index;
		_count += 1;
	}
	CHECKED bool tryUseSlot(TIndex index) {
		if (isFull()) {
			return false;
		}

		sample(index);
		return true;
	}
	void reset() {
		_front = 0;
		_count = 0;
	}

protected:
	size_t backLocation() const {
		const size_t back = _front + _count;
		return back >= size ? back - size : back;
	}

	//NOTE: This method assume that there is one to slide, so don't call it
	//if there's not...
	inline void forceSlideOne() {
		assert(_count > 0);

		_front += 1;
		if (_front == size) {
			_front = 0;
		}
		_count -= 1;
	}

	TIndex _duration;

	size_t _front = 0;
	size_t _count = 0;
	TIndex _indexes[size];
};

}
