
#include <iostream>

#include "Werk/Console/IpcConsoleServer.hpp"
#include "Werk/Version.hpp"

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: ./IpcConsoleCleanup <NAME>" << std::endl;
		return 1;
	} else if (0 == strcmp(argv[1], "--version")) {
		std::cout << "Version: " << werk::getVersion() << std::endl;
		return 0;
	}

	std::cout << "Removing " << argv[1] << "..." << std::endl;
	werk::IpcConsoleServer::remove(argv[1]);
	std::cout << "Removed " << argv[1] << "." << std::endl;

	return 0;
}
