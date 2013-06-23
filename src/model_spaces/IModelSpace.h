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
