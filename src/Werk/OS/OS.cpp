
#include "OS.hpp"

#include <unistd.h>

namespace werk
{

const char *getOperatingSystemName()
{
//TODO: more clever implementation
#ifdef __MACH__
	return "Mac";
#else
	return "Linux";
#endif
}

}
