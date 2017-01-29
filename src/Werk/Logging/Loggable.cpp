
#include "Loggable.hpp"

#include "Log.hpp"

namespace werk
{

void StringLoggable::logTo(Log *log) const {
	log->logRaw(_level, value().c_str());
}

}
