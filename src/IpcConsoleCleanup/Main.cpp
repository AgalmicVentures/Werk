
#include <iostream>

#include "Werk/Console/IpcConsoleServer.hpp"

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: ./IpcConsoleCleanup <NAME>" << std::endl;
		return 1;
	}

	std::cout << "Removing " << argv[1] << "..." << std::endl;
	werk::IpcConsoleServer::remove(argv[1]);
	std::cout << "Removed " << argv[1] << "." << std::endl;

	return 0;
}
