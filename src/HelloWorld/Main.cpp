
#include <cstdio>

#include "Werk/Utility/SummaryStatistics.hpp"

int main()
{
	werk::SummaryStatistics<double> s;
	s.sample(0.0);
	s.sample(1.0);
	s.sample(2.0);

	std::printf("Hello world! count=%llu average=%f", s.count(), s.average());
	return 0;
}
