
#pragma once

#include <cstdint>

namespace werk {

class IdGenerator
{
public:

	IdGenerator(uint64_t start=0, uint64_t step=1) : _step(step), _nextId(start) { }

	uint64_t step() const { return _step; }

	uint64_t nextId() const { return _nextId; }

	uint64_t getNext() {
		uint64_t id = _nextId;
		++_nextId;
		return id;
	}

private:
	//Config
	uint64_t _step;

	//State
	uint64_t _nextId;
};

}
