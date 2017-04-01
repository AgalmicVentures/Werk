
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Werk/Console/IpcConsoleClient.hpp"
#include "Werk/Version.hpp"

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: ./IpcConsoleClient <NAME>" << std::endl;
		return 1;
	} else if (0 == std::strcmp(argv[1], "--version")) {
		std::cout << "Version: " << werk::getVersion() << std::endl;
		return 0;
	}

	try {
		std::cout << "Connecting to " << argv[1] << "..." << std::endl;
		std::unique_ptr<werk::IpcConsoleClient> client(new werk::IpcConsoleClient(argv[1]));
		std::cout << "Connected to " << argv[1] << "." << std::endl;

		std::string input;
		std::vector<std::string> messages;
		while (std::getline(std::cin, input)) {
			if (input.empty()) {
				//Skip 0 length inputs
				continue;
			} else if (input == "reconnect") {
				//In some situations, the client may want to "reconnect", e.g. if the server goes down
				std::cout << "Reconnecting..." << std::endl;
				client.reset(new werk::IpcConsoleClient(argv[1]));
				std::cout << "Reconnected." << std::endl;
				continue;
			}

			//Parse the input into messages, splitting on ;
			messages.clear();
			boost::split(messages, input, boost::is_any_of(";"));
			for (auto &message : messages) {
				if (client->send(message)) {
					std::cout << "Sent '" << message << "'" << std::endl;

					if (message == "quit") {
						std::cout << "Received 'quit' command..." << std::endl;
						break;
					}
				} else {
					std::cout << "Failed to send '" << message << "'!" << std::endl;
				}
			}
		}
	} catch (boost::interprocess::interprocess_exception e) {
		std::cout << "IPC exception thrown, connection lost..." << std::endl;
	}

	return 0;
}
