
/*
 * Copyright (c) 2015-2021 Agalmic Ventures LLC (www.agalmicventures.com)
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

#include <atomic>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Werk/Console/IpcConsoleClient.hpp"
#include "Werk/Version.hpp"

void heartbeat(std::atomic<bool> *running, werk::IpcConsoleClient *client)
{
	timespec delay;
	delay.tv_sec = 1;
	delay.tv_nsec = 0;

	while (*running) {
		nanosleep(&delay, nullptr);
		client->heartbeat();
	}
}

/**
 * Sends a message, and returns false if time to quit.
 */
bool sendMessage(std::atomic<bool> *running, werk::IpcConsoleClient *client, const std::string &message)
{
	if (message == "consoleQuit") {
		std::cout << "Received 'consoleQuit' command..." << std::endl;
		*running = false;
		return false;
	}

	if (client->send(message)) {
		std::cout << "Sent '" << message << "'" << std::endl;

		if (message == "quit") {
			std::cout << "Received 'quit' command..." << std::endl;
			*running = false;
			return false;
		}
	} else {
		std::cout << "Failed to send '" << message << "'!" << std::endl;
	}

	return true;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: ./IpcConsoleClient <NAME> [<COMMAND1> ...]" << std::endl;
		return 1;
	} else if (0 == std::strcmp(argv[1], "--version")) {
		std::cout << "Version: " << werk::getVersion() << std::endl;
		return 0;
	}

	bool connected = false;
	try {
		std::cout << "Connecting to " << argv[1] << "..." << std::endl;
		werk::IpcConsoleClient *client = new werk::IpcConsoleClient(argv[1]);
		std::cout << "Connected to " << argv[1] << "." << std::endl;
		connected = true;

		//Run commands from arguments
		std::atomic<bool> running = true;
		std::vector<std::string> messages;
		for (uint32_t i = 2; i < argc; ++i) {
			if (!sendMessage(&running, client, argv[i])) {
				return 0;
			}
		}

		//Start the background thread
		std::thread heartbeatThread(heartbeat, &running, client);

		//Read input from the foreground
		std::string input;
		while (running && std::getline(std::cin, input)) {
			if (input.empty()) {
				//Skip 0 length inputs
				continue;
			}

			//Parse the input into messages, splitting on ;
			messages.clear();
			boost::split(messages, input, boost::is_any_of(";"));
			for (const auto &message : messages) {
				if (!sendMessage(&running, client, message)) {
					break;
				}
			}
		}

		heartbeatThread.join();
	} catch (boost::interprocess::interprocess_exception &e) {
		std::cout << "IPC exception thrown, " << (connected ? "connection lost" : "could not connect")
			<< "..." << std::endl;
	}

	return 0;
}
