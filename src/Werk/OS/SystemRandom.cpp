
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
	size_t n = std::fread(bytes, 1, size, _file);
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
