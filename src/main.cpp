#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "pixbuffer.h"
#include "ply_reader.h"
#include "profiler.h"
#include "tga_saver.h"
#include "vector.h"

int main(int argc, const char* argv[], const char* envp[])
{
	std::auto_ptr<PLY_Model> model;
    Profiler profiler;

#define PROFILE_START(msg) std::cout << msg << std::flush; profiler.start()
#define PROFILE_END() std::cout << (profiler.stop() / 1000.0) << " seconds " << std::endl

	// Load the PLY model
	const char* modelPath = "models/happy_vrip_res4.ply";

	PROFILE_START("Loading model " << modelPath << " ... ");
	model = Read_PLY_Model(modelPath);
	PROFILE_END();


    // Create a pixbuf as render targe
    PROFILE_START("Creating render target ... ");
    PixBuffer target ( 800, 600, 24 );
    target.clear ( 0x002244 );
    PROFILE_END();


    // Save the result
    PROFILE_START("Saving the result ... ");
    Write_Tga("output/result.tga", target.getWidth(), target.getHeight(), target.getBuffer());
    PROFILE_END();

#undef PROFILE_START
#undef PROFILE_END

	system("pause");
	return EXIT_SUCCESS;
}
