
#pragma once

namespace werk
{

class Action;

/**
 * Sets handlers for SIGSEGV and SIGBUS.
 */
bool setupSegfaultHandler();

/**
 * Sets up a signal handler for the given signal that executes the given action.
 */
bool setupSignalHandler(int signal, Action *action);

}
