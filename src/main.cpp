#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "vector.h"

int main(int argc, const char* argv[], const char* envp[])
{
	vec3f v ( 1.0f, 2.0f, 3.0f );

	v *= 2;

	std::cout << (std::string)v << std::endl;
	std::cout << length(v) << std::endl;

	system("pause");
	return EXIT_SUCCESS;
}
