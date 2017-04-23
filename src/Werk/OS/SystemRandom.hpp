
#pragma once

#include <cinttypes>
#include <cstdio>

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

class SystemRandom
{
public:
	SystemRandom();
	~SystemRandom();

	CHECKED bool getBytes(uint8_t *bytes, size_t size);
	CHECKED bool getInt8(int8_t &value);
	CHECKED bool getInt16(int16_t &value);
	CHECKED bool getInt32(int32_t &value);
	CHECKED bool getInt64(int64_t &value);
	CHECKED bool getUint8(uint8_t &value);
	CHECKED bool getUint16(uint16_t &value);
	CHECKED bool getUint32(uint32_t &value);
	CHECKED bool getUint64(uint64_t &value);

protected:
	FILE *_file;
};

}
