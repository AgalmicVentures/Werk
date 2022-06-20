
/*
 * Copyright (c) 2015-2022 Agalmic Ventures LLC (www.agalmicventures.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <cinttypes>
#include <cstdio>
#include <unistd.h>

#include "Werk/Application/Context.hpp"
#include "Werk/Logging/Loggable.hpp"
#include "Werk/Math/SummaryStatistics.hpp"
#include "Werk/Threading/Timer.hpp"

class ShutdownAction : public werk::Action
{
public:
	ShutdownAction(const std::string &name, werk::Log *log) : werk::Action(name), _log(log) { }
	virtual ~ShutdownAction() { }

	void execute() override { _log->logRaw(werk::LogLevel::SUCCESS, "Shutdown action!"); }
private:
	werk::Log *_log;
};

int main(int argc, char **argv)
{
	//Create the application context with a blank path to redirect to stdout
	werk::Context context(
		argc > 1 ? argv[1] : "src/HelloWorld/TestRealTime.ini",
		argc > 2 ? argv[2] : nullptr);

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

	return context.run();
}
