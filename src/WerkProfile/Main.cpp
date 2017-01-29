
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Logging/Log.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

void profileLog(werk::ProfileManager &profileManager)
{
	werk::BackgroundThread backgroundThread(nullptr, 1l * 1000 * 1000);

	FILE *file = fopen("/dev/null", "a");
	werk::AsyncLog *log = new werk::AsyncLog("Log", &backgroundThread.backgroundClock(), file);
	backgroundThread.addTask(log);

	//Raw logging
	werk::Profile *asyncRawProfile = new werk::Profile("AsyncLogRaw", 1000, 1000);
	profileManager.add(asyncRawProfile);
	for (size_t i = 0; i < 25000; ++i) {
		asyncRawProfile->start(werk::epochTime());
		log->logRaw(werk::LogLevel::INFO, "Testing raw log");
		asyncRawProfile->stop(werk::epochTime());
	}

	//Formatted logging
	werk::Profile *asyncProfile = new werk::Profile("AsyncLog", 1000, 1000);
	profileManager.add(asyncProfile);
	for (int i = 0; i < 25000; ++i) {
		asyncProfile->start(werk::epochTime());
		log->log(werk::LogLevel::INFO, "Testing log with formatting %f %d", 3.14, i);
		asyncProfile->stop(werk::epochTime());
	}
}

int main() {
	werk::ProfileManager profileManager;

	profileLog(profileManager);

	profileManager.writeJson(stdout);

	return 0;
}
