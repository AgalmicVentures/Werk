
#include "QuitCommand.hpp"

#include <cstdlib>

#include "ApplicationContext.hpp"

namespace werk
{

bool QuitCommand::execute(const std::vector<std::string> &) {
	_applicationContext->log()->logRaw(LogLevel::INFO, "Quitting...");
	_applicationContext->shutdown();
	std::exit(0);

	//Never reached
	return true;
}

}
