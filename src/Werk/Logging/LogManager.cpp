
#include "LogManager.hpp"

namespace werk
{

void LogManager::logTo(Log *log) const
{
	log->log(LogLevel::INFO, "<LogManager> Logs (%zu):", _logs.size());
	for (auto i = _logs.begin(); i != _logs.end(); ++i) {
		log->log(LogLevel::INFO, "  %24s", i->first.c_str());
	}
}

}
