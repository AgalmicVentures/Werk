
#include "PcapParser.hpp"

#include <boost/algorithm/string.hpp>

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
	//Read the next packet header
	_file.read((char *) &_recordHeader, sizeof(_recordHeader));
	_time = _recordHeader.timeSec * 1e9l +
		(_isNanosecond ? _recordHeader.timeFrac : _recordHeader.timeFrac * 1e3l);

	//And the length
	if (!_file.good()) {
		return false;
	}
	if (_recordHeader.length > 65535) {
		return false;
	}

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
