
#include <cinttypes>
#include <cstdio>

#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Math/SummaryStatistics.hpp"

int main()
{
	//Create the application context with a blank path to redirect to stdout
	werk::ApplicationContext context("");

	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	context.log()->log(werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f",
		s.count(), s.average(), s.stddev());

	//This will be called automatically when the context falls out of scope, but is left in as an example
	context.shutdown();

	return 0;
}
