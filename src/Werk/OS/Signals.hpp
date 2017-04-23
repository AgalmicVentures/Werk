
#pragma once

#include "Werk/Utility/Attributes.hpp"

namespace werk
{

class Action;

/**
 * Sets handlers for SIGSEGV and SIGBUS.
 */
CHECKED bool setupSegfaultHandler();

/**
 * Sets up a signal handler for the given signal that executes the given action.
 */
CHECKED bool setupSignalHandler(int signal, Action *action);

}
