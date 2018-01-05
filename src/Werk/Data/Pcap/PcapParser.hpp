
/*
 * Copyright (c) 2015-2018 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <fstream>
#include <string>
#include <vector>

#include "Werk/Data/TimeSeries.hpp"
#include "Werk/Utility/Attributes.hpp"

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

	virtual ~PcapParser() { close(); }

	CHECKED bool isOpen() const { return _file.is_open(); }
	CHECKED bool open(const std::string &path);
	void close();

	CHECKED uint32_t magicNumber() const { return _fileHeader.magicNumber; }
	CHECKED uint16_t versionMajor() const { return _fileHeader.versionMajor; }
	CHECKED uint16_t versionMinor() const { return _fileHeader.versionMinor; }
	CHECKED int32_t tzOffset() const { return _fileHeader.tzOffset; }
	CHECKED uint32_t sigfigs() const { return _fileHeader.sigfigs; }
	CHECKED uint32_t snaplen() const { return _fileHeader.snaplen; }
	CHECKED uint32_t network() const { return _fileHeader.network; }
	CHECKED bool isNanosecond() const { return _isNanosecond; }

	CHECKED uint32_t length() const { return _recordHeader.length; }
	CHECKED uint32_t originalLength() const { return _recordHeader.originalLength; }
	CHECKED const uint8_t *buffer() const { return _buffer; }

	CHECKED virtual uint64_t time() const override { return _time; }
	CHECKED virtual bool moveNext() override;

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
