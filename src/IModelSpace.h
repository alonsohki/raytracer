#pragma once

#include "ray.h"
#include "vector.h"

class IModelSpace
{
public:
    virtual         ~IModelSpace    () {}

    virtual void    load            ( const float* vertices, unsigned int vertexCount, const int* faces, unsigned int faceCount );
    virtual bool    intersect       ( const Ray& ray, vec3f* pos, vec3f* normal );
};
