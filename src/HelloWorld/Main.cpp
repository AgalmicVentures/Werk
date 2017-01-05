
#include <cinttypes>
#include <cstdio>

#include "Werk/Logging/AsyncLogger.hpp"
#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

int main()
{
	werk::Clock clock;
	clock.setEpochTime();

	werk::AsyncLogger *log = new werk::AsyncLogger(&clock);

	werk::BackgroundThread *backgroundThread = new werk::BackgroundThread();
	backgroundThread->addTask(log);

	log->logRaw(werk::LogLevel::INFO, "Starting...");

	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	log->log(werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f", s.count(), s.average(), s.stddev());

	backgroundThread->stop();
	return 0;
}
