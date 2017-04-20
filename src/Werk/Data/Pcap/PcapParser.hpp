
#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Werk/Data/TimeSeries.hpp"

namespace werk
{

//TODO: support swapped pcaps

constexpr uint64_t PCAP_MAGIC_NUMBER = 0xa1b2c3d4;
constexpr uint64_t PCAP_NS_MAGIC_NUMBER = 0xa1b23c4d;
constexpr uint64_t PCAP_MAGIC_NUMBER_INVERTED = 0xd4c3b2a1;
constexpr uint64_t PCAP_NS_MAGIC_NUMBER_INVERTED = 0x4d3cb2a1;

struct PcapHeader
{
	uint32_t magicNumber;
	uint16_t versionMajor;
	uint16_t versionMinor;
	int32_t tzOffset;
	uint32_t sigfigs;
	uint32_t snaplen;
	uint32_t network;
};

struct PcapRecordHeader
{
	uint32_t timeSec;
	uint32_t timeFrac;
	uint32_t length;
	uint32_t originalLength;
};

/**
 * Parses a PCAP file into string values.
 */
class PcapParser : public TimeSeries
{
public:

	bool isOpen() const { return _file.is_open(); }
	bool open(const std::string &path);
	void close();

	uint32_t magicNumber() const { return _fileHeader.magicNumber; }
	uint16_t versionMajor() const { return _fileHeader.versionMajor; }
	uint16_t versionMinor() const { return _fileHeader.versionMinor; }
	int32_t tzOffset() const { return _fileHeader.tzOffset; }
	uint32_t sigfigs() const { return _fileHeader.sigfigs; }
	uint32_t snaplen() const { return _fileHeader.snaplen; }
	uint32_t network() const { return _fileHeader.network; }
	bool isNanosecond() const { return _isNanosecond; }

	uint32_t length() const { return _recordHeader.length; }
	uint32_t originalLength() const { return _recordHeader.originalLength; }
	const uint8_t *buffer() const { return _buffer; }

	virtual uint64_t time() const override { return _time; }
	virtual bool moveNext() override;

protected:
	std::ifstream _file;

	//File header
	PcapHeader _fileHeader;
	bool _isNanosecond;

	//Most recent packet
	PcapRecordHeader _recordHeader;
	uint64_t _time = 0;
	uint8_t _buffer[65536];
};

}
