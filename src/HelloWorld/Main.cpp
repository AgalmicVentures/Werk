
#include <cinttypes>
#include <cstdio>

#include "Werk/Math/SummaryStatistics.hpp"

int main()
{
	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	std::printf("Hello world! count=%" PRIu64 " average=%f stddev=%f", s.count(), s.average(), s.stddev());
	return 0;
}
