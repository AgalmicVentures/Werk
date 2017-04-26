
/*
 * Copyright (c) 2015-2017 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <cinttypes>

namespace werk
{

/**
 * Base sliding window implementation which delegates how to handle adding and
 * removing elements by delegating to an implementation class.
 *
 * NOTE: Implementing class MUST provide onAddValue and onRemoveValue methods.
 */
template <typename TIndex, typename TValue, size_t size, typename TImpl>
class BaseSlidingWindow
{
public:
	BaseSlidingWindow(TIndex duration) : _duration(duration) { }

	TIndex duration() const { return _duration; }

	size_t count() const { return _count; }
	bool isEmpty() const { return _count == 0; }
	bool isFull() const { return _count == size; }

	//NOTE: These are not bounds checked since there is no sensible default
	TIndex frontIndex() const { return _indexes[_front]; }
	TValue front() const { return _values[_front]; }

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
	void sample(TIndex index, TValue value) {
		//Make room if necessary
		if (isFull()) {
			forceSlideOne();
		}

		const size_t back = backLocation();
		_indexes[back] = index;
		_values[back] = value;
		_count += 1;

		static_cast<TImpl *>(this)->onAddValue(index, value);
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

		static_cast<TImpl *>(this)->onRemoveValue(_indexes[_front], _values[_front]);

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
	TValue _values[size];

	typedef BaseSlidingWindow<TIndex, TValue, size, TImpl> _BaseSlidingWindow;
};

/**
 * Just a sliding window, nothing more.
 */
template <typename TIndex, typename TValue, size_t size>
class SlidingWindow : public BaseSlidingWindow<TIndex, TValue, size, SlidingWindow<TIndex, TValue, size> >
{
public:
	SlidingWindow(TIndex duration) : BaseSlidingWindow<TIndex, TValue, size, SlidingWindow<TIndex, TValue, size> >(duration) { }

	void onAddValue(TIndex index, TValue value) {
		(void) index;
		(void) value;
	}
	void onRemoveValue(TIndex index, TValue value) {
		(void) index;
		(void) value;
	}
};

/**
 * A sliding window that keeps a sum.
 */
template <typename TIndex, typename TValue, size_t size>
class SumSlidingWindow : public BaseSlidingWindow<TIndex, TValue, size, SumSlidingWindow<TIndex, TValue, size> >
{
public:
	SumSlidingWindow(TIndex duration) : BaseSlidingWindow<TIndex, TValue, size, SumSlidingWindow<TIndex, TValue, size> >(duration) { }

	TValue sum() const { return _sum; }

	void onAddValue(TIndex index, TValue value) {
		(void) index;
		_sum += value;
	}
	void onRemoveValue(TIndex index, TValue value) {
		(void) index;
		_sum -= value;
	}

protected:
	TValue _sum = 0;
};

}
