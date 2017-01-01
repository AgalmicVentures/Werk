
#include "Werk/Profiling/ProfileManager.hpp"

int main() {
	werk::ProfileManager profileManager;

	profileManager.writeJson(stdout);

	return 0;
}