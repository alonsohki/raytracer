//
// FILE:        Main.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Application entry point.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Config.h"
#include "MathUtil.h"
#include "model_spaces/BruteForce.h"
#include "model_spaces/KDTreeSpace.h"
#include "Pixbuffer.h"
#include "ply_reader.h"
#include "Profiler.h"
#include "Renderer.h"
#include "test/TestOutput.h"
#include "tga_saver.h"
#include "Vector.h"

int main(int argc, const char* argv[], const char* envp[])
{
	std::auto_ptr<PLY_Model> model;
    Profiler profiler;

#define PROFILE_START(msg) std::cout << msg << std::flush; profiler.start()
#define PROFILE_END() std::cout << (profiler.stop() / 1000.0) << " seconds " << std::endl

	// Load the PLY model
	const char* modelPath = MODEL_PATH;

	PROFILE_START("Loading model " << modelPath << " ... ");
	model = Read_PLY_Model(modelPath);
	PROFILE_END();

    // Let's create the model space with the loaded model data.
    PROFILE_START("Building model space ... ");
#ifndef USE_KDTREE
    ModelSpaces::BruteForce modelSpace;
#else
    ModelSpaces::KDTreeSpace modelSpace;
#endif
    modelSpace.load ( &model->m_vertex_data[0], model->Get_Vertex_Count(),
                      &model->m_face_data[0], model->Get_Face_Count() );
    PROFILE_END();

    // Create a pixbuf as render target
    PROFILE_START("Creating render target ... ");
    PixBuffer target ( TARGET_WIDTH, TARGET_HEIGHT, 24 );
    target.clear ( 0x000000 );
    PROFILE_END();


    // Render!
    PROFILE_START("Rendering ... ");
    Renderer renderer;
    renderer.renderModel(&target, &modelSpace);
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


    // Run the test case
    printf ( "Testing ... " );
    fflush ( stdout );
    Test::TestOutput test;
    if ( test.compareOutput("output/result.tga", "output/expected_result.tga") )
        puts("OK!");
    else
    {
        printf("Error: %s\n", test.getError());
    }

#undef PROFILE_START
#undef PROFILE_END

	system("pause");
	return EXIT_SUCCESS;
}
