
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

#include "Action.hpp"
#include "Attributes.hpp"

namespace werk
{

/**
 * A set-reset latch.
 *
 * Templated on type so that the boolean can be made atomic if necessary.
 */
template <typename T=bool>
class Latch
{
public:
	Latch(bool value=false) : _value(value) { }

	CHECKED bool value() const { return _value; }
	void set() { _value = true; }
	void reset() { _value = false; }

private:
	T _value;
};

/**
 * An action that sets a `Latch`. To reset, use a generic `ResetAction`.
 */
template <typename T=bool>
class SetLatchAction : public Action
{
public:
	SetLatchAction(const std::string &name, Latch<T> &latch) :
		Action(name), _latch(latch) { }

	void execute() override { _latch.set(); }

private:
	Latch<T> &_latch;
};

/**
 * An action that executes another action, if the latch is set.
 */
template <typename T=bool>
class ConditionalAction : public Action
{
public:
	ConditionalAction(const std::string &name, Latch<T> &latch, Action *action) :
		Action(name), _latch(latch), _action(action) { }

	void execute() override {
		if (_latch.value()) {
			_action->execute();
		}
	}

private:
	Latch<T> &_latch;
	Action *_action;
};

}
