
#include "Signals.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <signal.h>

namespace werk
{

static void handleBusError(int /*signal*/, siginfo_t *info, void * /*context*/)
{
    const char *cause;
    switch (info->si_code) {
    case BUS_ADRALN: cause = "Incorrect Memory Alignment";  break;
    case BUS_ADRERR: cause = "Nonexistent Physical Address"; break;
    case BUS_OBJERR: cause = "Object Error (Hardware)"; break;
    default: cause = "Unknown";
    }

    std::printf("\n\n******************** Bus Error ********************\n\n");
    std::printf("Faulting address: 0x%lx\nCause: %s (%d)\n", (intptr_t) info->si_addr, cause, info->si_code);

    std::abort();
}

static void handleSegfault(int /*signal*/, siginfo_t *info, void * /*context*/)
{
    const char *cause;
    switch (info->si_code) {
    case SEGV_ACCERR: cause = "Access Permissions Error";  break;
    case SEGV_MAPERR: cause = "Map Error"; break;
    default: cause = "Unknown";
    }

	std::printf("\n\n******************** Segmentation Fault ********************\n\n");
	std::printf("Faulting address: 0x%lx\nCause: %s (%d)\n", (intptr_t) info->si_addr, cause, info->si_code);

	std::abort();
}

bool setupSegfaultHandler()
{
    struct sigaction sa;

    std::memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handleSegfault;
    sa.sa_flags = SA_SIGINFO;
    if (0 > sigaction(SIGSEGV, &sa, nullptr)) {
        //TODO: log
        return false;
    }

    std::memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handleBusError;
    sa.sa_flags = SA_SIGINFO;
    if (0 > sigaction(SIGBUS, &sa, nullptr)) {
        //TODO: log
        return false;
    }

    return true;
}

}
