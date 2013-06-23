//
// FILE:        Main.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Application entry point.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "MathUtil.h"
#include "Pixbuffer.h"
#include "ply_reader.h"
#include "Profiler.h"
#include "tga_saver.h"
#include "Vector.h"

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


    // Create a pixbuf as render target
    PROFILE_START("Creating render target ... ");
    PixBuffer target ( 800, 600, 24 );
    target.clear ( 0x000000 );
    PROFILE_END();


    // Save the result
    PROFILE_START("Saving the result ... ");
    // Flip the image vertically
    PixBuffer result ( target.getWidth(), target.getHeight(), target.getDepth() );
    for ( unsigned int j = 0; j < target.getHeight(); ++j )
        for ( unsigned int i = 0; i < target.getWidth(); ++i )
            result.setPixel(i, target.getHeight() - j - 1, target.getPixel(i, j));
    Write_Tga("output/result.tga", result.getWidth(), result.getHeight(), result.getBuffer());
    PROFILE_END();

#undef PROFILE_START
#undef PROFILE_END

	system("pause");
	return EXIT_SUCCESS;
}
