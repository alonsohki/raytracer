//
// FILE:        BoundingBox.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Bounding boxes
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "BoundingBox.h"
#include <stdarg.h>


BoundingBox BoundingBox::calculateFromVertices ( const vec3f* vertices, int* indices, unsigned int indexCount )
{
    BoundingBox result;
    
    float inf = std::numeric_limits<float>::infinity();
    result.min = vec3f(inf, inf, inf);
    result.max = vec3f(-inf, -inf, -inf);

    for ( unsigned int i = 0; i < indexCount; ++i )
    {
        const vec3f& v = vertices[indices[i]];
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

BoundingBox BoundingBox::calculateFromVertices ( const vec3f* vertices, unsigned int count, int v1, ... )
{
    BoundingBox result;
    va_list vl;
    va_start ( vl, v1 );
    
    float inf = std::numeric_limits<float>::infinity();
    result.min = vec3f(inf, inf, inf);
    result.max = vec3f(-inf, -inf, -inf);

    int current = v1;
    do
    {
        const vec3f& v = vertices[current];
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

        current = va_arg(vl, int);
    }
    while ( --count > 0 );

    va_end ( vl );

    return result;
}

BoundingBox BoundingBox::calculateFromVertices ( const vec3f* vertices, unsigned int vertexCount )
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