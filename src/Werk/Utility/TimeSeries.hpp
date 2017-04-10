
#pragma once

namespace werk
{

class TimeSeries
{
public:

	virtual uint64_t time() = 0;
	virtual uint64_t nextTime() = 0;

	virtual bool moveNext() = 0;
};

}
