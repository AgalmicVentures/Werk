
#include "Werk/Logging/AsyncLog.hpp"
#include "Werk/Logging/Log.hpp"
#include "Werk/OS/Time.hpp"
#include "Werk/Profiling/ProfileManager.hpp"
#include "Werk/Threading/BackgroundThread.hpp"

static void profileAllocations(werk::ProfileManager &profileManager)
{
	//N-byte new + delete
	const int iterations = 100000;
	const int sizes[] = {1, 16, 256, 2048, 65536, 1024 * 1024};
	for (const int size : sizes) {
		werk::Profile *newDeleteProfile = new werk::Profile(std::string("NewDelete") + std::to_string(size), 1000, 1000);

		profileManager.add(newDeleteProfile);
		for (size_t i = 0; i < iterations; ++i) {
			PROFILE_START(*newDeleteProfile);
			uint8_t *buffer = new uint8_t[size];
			delete [] buffer;
			PROFILE_STOP(*newDeleteProfile);
		}
	}

	//K N-byte news then deletes
	const int allocations = 1000;
	uint8_t **buffers = new uint8_t*[allocations];
	for (const int size : sizes) {
		werk::Profile *newProfile = new werk::Profile(std::string("NewDelete_New") + std::to_string(size), 1000, 1000);
		werk::Profile *deleteProfile = new werk::Profile(std::string("NewDelete_Delete") + std::to_string(size), 1000, 1000);

		profileManager.add(newProfile);
		for (size_t i = 0; i < allocations; ++i) {
			PROFILE_START(*newProfile);
			buffers[i] = new uint8_t[size];
			PROFILE_STOP(*newProfile);
		}

		profileManager.add(deleteProfile);
		for (size_t i = 0; i < allocations; ++i) {
			PROFILE_START(*deleteProfile);
			delete [] buffers[i];
			PROFILE_STOP(*deleteProfile);
		}
	}
	delete [] buffers;
}

static void profileIO(werk::ProfileManager &profileManager)
{
	const int iterations = 25000;

	//Raw logging
	FILE *devNull = std::fopen("/dev/null", "a");
	werk::Profile *devNullProfile = new werk::Profile("WriteDevNull", 1000, 1000);
	profileManager.add(devNullProfile);
	for (size_t i = 0; i < iterations; ++i) {
		PROFILE_START(*devNullProfile);
		std::fprintf(devNull, "01234567890 123456789\n");
		PROFILE_STOP(*devNullProfile);
	}
	std::fclose(devNull);
}

static void profileLog(werk::ProfileManager &profileManager)
{
	werk::BackgroundThread backgroundThread(nullptr, 1l * 1000 * 1000);

	FILE *file = std::fopen("/dev/null", "a");
	werk::AsyncLog *log = new werk::AsyncLog("Log", &backgroundThread.backgroundClock(), file);
	backgroundThread.addTask(log);

	const int iterations = 25000;

	//Raw logging
	werk::Profile *asyncRawProfile = new werk::Profile("AsyncLogRaw", 1000, 1000);
	profileManager.add(asyncRawProfile);
	for (size_t i = 0; i < iterations; ++i) {
		PROFILE_START(*asyncRawProfile);
		log->logRaw(werk::LogLevel::INFO, "Testing raw log");
		PROFILE_STOP(*asyncRawProfile);
	}

	//Formatted logging
	werk::Profile *asyncProfile = new werk::Profile("AsyncLog", 1000, 1000);
	profileManager.add(asyncProfile);
	for (int i = 0; i < iterations; ++i) {
		PROFILE_START(*asyncProfile);
		log->log(werk::LogLevel::INFO, "Testing log with formatting %f %d", 3.14, i);
		PROFILE_STOP(*asyncProfile);
	}

	backgroundThread.stop();
	std::fclose(file);
}

int main() {
	werk::ProfileManager profileManager;

	profileAllocations(profileManager);
	profileIO(profileManager);
	profileLog(profileManager);

	profileManager.writeJson(stdout);

	return 0;
}
