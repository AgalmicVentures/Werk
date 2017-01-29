
#include <cinttypes>
#include <cstdio>
#include <unistd.h>

#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/Threading/Timer.hpp"

class ShutdownAction : public werk::Action
{
public:
	ShutdownAction(const std::string &name, werk::Log *log) : werk::Action(name), _log(log) { }

	void execute() override { _log->logRaw(werk::LogLevel::SUCCESS, "Shutdown action!"); }
private:
	werk::Log *_log;
};

int main()
{
	//Create the application context with a blank path to redirect to stdout
	werk::ApplicationContext context("src/HelloWorld/Test.ini");

	werk::StringLoggable sl("Checking in...");
	werk::Timer timer("Timer", &context.backgroundThread().backgroundClock(),
		new werk::LogAction("TimerLog", &sl, context.log()),
		60l * 1000 * 1000 * 1000);
	context.backgroundThread().addTask(&timer);

	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	context.log()->log(werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f",
		s.count(), s.average(), s.stddev());

	context.commandManager()->execute("help");

	context.shutdownActions().push_back(new ShutdownAction("Shutdown", context.log()));

	context.run();

	return 0;
}
