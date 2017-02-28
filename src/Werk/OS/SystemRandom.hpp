
#pragma once

#include <cinttypes>
#include <cstdio>

namespace werk
{

class SystemRandom
{
public:
	SystemRandom();
	~SystemRandom();

	bool getBytes(uint8_t *bytes, size_t size);
	bool getInt8(int8_t &value);
	bool getInt16(int16_t &value);
	bool getInt32(int32_t &value);
	bool getInt64(int64_t &value);
	bool getUint8(uint8_t &value);
	bool getUint16(uint16_t &value);
	bool getUint32(uint32_t &value);
	bool getUint64(uint64_t &value);

protected:
	FILE *_file;
};

}
