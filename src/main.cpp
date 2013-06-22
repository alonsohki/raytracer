#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "ply_reader.h"
#include "profiler.h"
#include "vector.h"

int main(int argc, const char* argv[], const char* envp[])
{
	Profiler profiler;
	std::auto_ptr<PLY_Model> model;
	
	const char* models[] = { "models/happy_vrip.ply",
		                     "models/happy_vrip_res2.ply",
						     "models/happy_vrip_res3.ply",
						     "models/happy_vrip_res4.ply" };
	for ( auto model : models )
	{
		std::cout << "Loading model " << model << std::flush;
		profiler.start();
		Read_PLY_Model(model);
		std::cout << " took: " << (profiler.stop() / 1000.0) << " seconds " << std::endl;
	}

	system("pause");
	return EXIT_SUCCESS;
}
