
#include <cinttypes>
#include <cstdio>

#include "Werk/Logging/Logger.hpp"
#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"

int main()
{
	werk::Clock clock;
	clock.setEpochTime();

	werk::Logger *log = new werk::SyncLogger(&clock);
	log->logRaw(werk::LogLevel::INFO, "Starting...");

	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	log->log(werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f", s.count(), s.average(), s.stddev());
	return 0;
}
