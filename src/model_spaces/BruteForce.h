//
// FILE:        BruteForce.h
// LICENSE:     The MIT license
// PURPOUSE:    Brute-force model space: No optimization at all in the collision detection.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "IModelSpace.h"

namespace ModelSpaces
{
    class BruteForce : public IModelSpace
    {
                        BruteForce      ();
        virtual         ~BruteForce     ();

        virtual void    load            ( const float* vertices, unsigned int vertexCount, const int* faces, unsigned int faceCount );
        virtual bool    intersect       ( const Ray& ray, vec3f* pos, vec3f* normal );
    };
}
