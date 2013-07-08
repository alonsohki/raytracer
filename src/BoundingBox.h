//
// FILE:        BoundingBox.h
// LICENSE:     The MIT license
// PURPOUSE:    Bounding boxes
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "Vector.h"

struct BoundingBox
{
    vec3f min;
    vec3f max;

    static BoundingBox calculateFromVertices ( const vec3f* vertices, unsigned int vertexCount )
    {
        BoundingBox result;
        float inf = std::numeric_limits<float>::infinity();
        result.min = vec3f(inf, inf, inf);
        result.max = vec3f(-inf, -inf, -inf);

        for ( unsigned int i = 0; i < vertexCount; ++i )
        {
            const vec3f& v = vertices[i];
            if ( v.x() < result.min.x() )
                result.min.x() = v.x();
            if ( v.x() > result.max.x() )
                result.max.x() = v.x();

            if ( v.y() < result.min.y() )
                result.min.y() = v.y();
            if ( v.y() > result.max.y() )
                result.max.y() = v.y();

            if ( v.z() < result.min.z() )
                result.min.z() = v.z();
            if ( v.z() > result.max.z() )
                result.max.z() = v.z();
        }

        return result;
    }

    bool contains ( const vec3f& point ) const
    {
        return point.x() >= min.x() &&
               point.x() <= max.x() &&
               point.y() >= min.y() &&
               point.y() <= max.y() &&
               point.z() >= min.z() &&
               point.z() <= max.z();
    }
};
