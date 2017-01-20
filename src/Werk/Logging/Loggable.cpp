
#include "Loggable.hpp"

#include "Log.hpp"

namespace werk
{

void StringLoggable::logTo(Log *log) const {
	log->logRaw(LogLevel::INFO, value().c_str());
}

}
