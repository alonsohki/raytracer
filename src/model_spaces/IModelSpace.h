//
// FILE:        IModelSpace.h
// LICENSE:     The MIT license
// PURPOUSE:    Interface for model spaces defining the necessary functions to load
//              models and to perform the ray-tracing on them.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../shapes/Ray.h"
#include "../Vector.h"

class IModelSpace
{
public:
    virtual         ~IModelSpace    () {}

    virtual void    load            ( const float* vertices, unsigned int vertexCount, const int* faces, unsigned int faceCount ) = 0;
    virtual bool    intersect       ( const Ray& ray, vec3f* pos, vec3f* normal ) = 0;
};
