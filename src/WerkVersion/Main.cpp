
#include <iostream>

#include "Werk/Version.hpp"

int main(int, char **)
{
	std::cout << werk::getVersion() << std::endl;
	return 0;
}
