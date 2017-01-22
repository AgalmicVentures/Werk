
#pragma once

namespace werk
{

/**
 * Represents a single message from a console client to a console server.
 */
struct ConsoleMessage
{
	//TODO: client ID or something?
	uint32_t sequenceNumber;
	char message[252];
};

}
