
#include <cinttypes>
#include <cstdio>
#include <unistd.h>

#include "Werk/Application/ApplicationContext.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/Math/SummaryStatistics.hpp"

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

	werk::SummaryStatistics<double> s;
	s.sample(5.0);
	s.sample(1.0);

	context.log()->log(werk::LogLevel::SUCCESS, "Hello world! count=%" PRIu64 " average=%f stddev=%f",
		s.count(), s.average(), s.stddev());

	context.commandManager()->execute("help");

	werk::StringLoggable sl("This is a StringLoggable test...");
	sl.logTo(context.log());

	context.log()->logRaw(werk::LogLevel::INFO, "Sleeping...");
	sleep(5);
	context.log()->logRaw(werk::LogLevel::INFO, "Slept.");

	//The following log line will never be printed since it quits here; however, the shutdown action will execute and log
	context.shutdownActions().push_back(new ShutdownAction("Shutdown", context.log()));
	context.commandManager()->execute("quit");

	context.log()->logRaw(werk::LogLevel::INFO, "Done.");

	//This will be called automatically when the context falls out of scope, but is left in as an example
	context.shutdown();

	return 0;
}
