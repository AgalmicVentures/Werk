
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

#include "SystemRandom.hpp"

namespace werk
{

SystemRandom::SystemRandom()
{
	//Try urandom first since it's just as good (if you trust your crypto
	//primitives that is)
	_file = std::fopen("/dev/urandom", "rb");
	if (nullptr == _file) {
		_file = std::fopen("/dev/random", "rb");
		if (nullptr == _file) {
			//TODO: error handling
		}
	}
}

SystemRandom::~SystemRandom()
{
	if (nullptr != _file) {
		std::fclose(_file);
	}
}

bool SystemRandom::getBytes(uint8_t *bytes, size_t size)
{
	const size_t n = std::fread(bytes, 1, size, _file);
	return size == n;
}

bool SystemRandom::getInt8(int8_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 1);
}

bool SystemRandom::getInt16(int16_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 2);
}

bool SystemRandom::getInt32(int32_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 4);
}

bool SystemRandom::getInt64(int64_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 8);
}

bool SystemRandom::getUint8(uint8_t &value)
{
	return getBytes(&value, 1);
}

bool SystemRandom::getUint16(uint16_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 2);
}

bool SystemRandom::getUint32(uint32_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 4);
}

bool SystemRandom::getUint64(uint64_t &value)
{
	return getBytes(reinterpret_cast<uint8_t *>(&value), 8);
}

}
