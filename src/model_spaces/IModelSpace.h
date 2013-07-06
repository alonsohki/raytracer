//
// FILE:        IModelSpace.h
// LICENSE:     The MIT license
// PURPOUSE:    Interface for model spaces defining the necessary functions to load
//              models and to perform the ray-tracing on them. Model spaces are meant
//              to transparently make the space partitioning, plus polygon data
//              calculation (normals).
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../Collision.h"
#include "../Face.h"
#include "../Ray.h"
#include "../Vector.h"

class IModelSpace
{
public:
    virtual         ~IModelSpace    () {}

    virtual void    load            ( const vec3f* vertices, unsigned int vertexCount, const Face* faces, unsigned int faceCount ) = 0;
    virtual bool    intersect       ( const Ray& ray, Collision* collision ) = 0;
};
