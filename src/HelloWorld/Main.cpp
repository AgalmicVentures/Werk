
#include <cinttypes>
#include <cstdio>

#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Math/SummaryStatistics.hpp"

int main()
{
	werk::ApplicationContext context("");

	context.log()->logRaw(werk::LogLevel::INFO, "Starting...");

	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	context.log()->log(werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f",
		s.count(), s.average(), s.stddev());

	context.backgroundThread().stop();
	return 0;
}
