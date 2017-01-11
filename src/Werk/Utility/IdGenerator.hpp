
#pragma once

#include <cstdint>

namespace werk {

/**
 * Generator for monotone ID's.
 */
template <typename T=uint64_t>
class IdGenerator
{
public:

	IdGenerator(T start=0, T step=1) : _step(step), _nextId(start) { }

	T step() const { return _step; }

	T nextId() const { return _nextId; }

	T getNext() {
		T id = _nextId;
		++_nextId;
		return id;
	}

private:
	//Config
	T _step;

	//State
	T _nextId;
};

}
