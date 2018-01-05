
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

#include "PcapParser.hpp"

namespace werk
{

bool PcapParser::open(const std::string &path)
{
	if (_file.is_open()) {
		return false;
	}

	_file.open(path, std::ios::in | std::ios::binary);
	if (!_file.is_open()) {
		return false;
	}

	//Read the header
	_file.read((char *) &_fileHeader, sizeof(_fileHeader));
	_isNanosecond = _fileHeader.magicNumber == PCAP_NS_MAGIC_NUMBER;

	return _file.good();
}

bool PcapParser::moveNext()
{
	//Read and validate the next packet header
	_file.read((char *) &_recordHeader, sizeof(_recordHeader));
	if (!_file.good()) {
		return false;
	}
	if (_recordHeader.length > 65535) {
		return false;
	}
	_time = _recordHeader.timeSec * 1000000000llu +
		(_isNanosecond ? _recordHeader.timeFrac : _recordHeader.timeFrac * 1000llu);

	//And the packet data
	_file.read((char *) _buffer, _recordHeader.length);

	return _file.good();
}

void PcapParser::close()
{
	if (_file.is_open()) {
		_file.close();
	}
}

}
