//
// FILE:        BoundingBox.h
// LICENSE:     The MIT license
// PURPOUSE:    Bounding boxes
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "Face.h"
#include "Ray.h"
#include <stdarg.h>
#include "Vector.h"

struct BoundingBox
{
    vec3f min;
    vec3f max;

    static BoundingBox calculateFromFaces    ( const vec3f* vertices, const Face* faces, int* indices, unsigned int indexCount );
    static BoundingBox calculateFromVertices ( const vec3f* vertices, int* indices, unsigned int indexCount );
    static BoundingBox calculateFromVertices ( const vec3f* vertices, unsigned int count, int v1, ... );
    static BoundingBox calculateFromVertices ( const vec3f* vertices, unsigned int vertexCount );

    bool intersect ( const BoundingBox& other ) const;
    bool intersect ( const Ray& ray, float* tmin, float* tmax ) const;

    bool contains ( const vec3f& point ) const
    {
        return point.x() >= min.x() &&
               point.x() <= max.x() &&
               point.y() >= min.y() &&
               point.y() <= max.y() &&
               point.z() >= min.z() &&
               point.z() <= max.z();
    }

    float width () const
    {
        return max.x() - min.x();
    }

    float height () const
    {
        return max.y() - min.y();
    }

    float depth () const
    {
        return max.z() - min.z();
    }

    double volume () const
    {
        return (double)width() * height() * depth();
    }
};
