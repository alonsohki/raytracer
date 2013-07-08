//
// FILE:        BoundingBox.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Bounding boxes
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "BoundingBox.h"
#include "MathUtil.h"
#include <stdarg.h>

bool BoundingBox::intersect ( const BoundingBox& other ) const
{
    return ( ( other.min.x() >= min.x() && other.min.x() <= max.x() ) || ( other.max.x() >= min.x() && other.max.x() <= max.x() ) &&
             ( other.min.y() >= min.y() && other.min.y() <= max.y() ) || ( other.max.y() >= min.y() && other.max.y() <= max.y() ) &&
             ( other.min.z() >= min.z() && other.min.z() <= max.z() ) || ( other.max.z() >= min.z() && other.max.z() <= max.z() ) );
}

bool BoundingBox::intersect ( const Ray& ray, float* tmin, float* tmax ) const
{
    vec3f dir = ray.delta;
    vec3f invdir = vec3f(1.0f/dir.x(), 1.0f/dir.y(), 1.0f/dir.z());

    float t1 = (min.x() - ray.origin.x())*invdir.x();
    float t2 = (max.x() - ray.origin.x())*invdir.x();
    float t3 = (min.y() - ray.origin.y())*invdir.y();
    float t4 = (max.y() - ray.origin.y())*invdir.y();
    float t5 = (min.z() - ray.origin.z())*invdir.z();
    float t6 = (max.z() - ray.origin.z())*invdir.z();

    *tmin = ::max(::max(::min(t1, t2), ::min(t3, t4)), ::min(t5, t6));
    *tmax = ::min(::min(::max(t1, t2), ::max(t3, t4)), ::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (*tmax < 0)
    {
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (*tmin > *tmax)
    {
        return false;
    }

    return true;
}

BoundingBox BoundingBox::calculateFromFaces ( const vec3f* vertices, const Face* faces, int* indices, unsigned int indexCount )
{
    BoundingBox result;
    
    float inf = std::numeric_limits<float>::infinity();
    result.min = vec3f(inf, inf, inf);
    result.max = vec3f(-inf, -inf, -inf);

    for ( unsigned int i = 0; i < indexCount; ++i )
    {
#define CHECK_VERTEX(from) \
        { \
            const vec3f& v = from; \
            if ( v.x() < result.min.x() ) \
                result.min.x() = v.x(); \
            if ( v.x() > result.max.x() ) \
                result.max.x() = v.x(); \
            if ( v.y() < result.min.y() ) \
                result.min.y() = v.y(); \
            if ( v.y() > result.max.y() ) \
                result.max.y() = v.y(); \
            if ( v.z() < result.min.z() ) \
                result.min.z() = v.z(); \
            if ( v.z() > result.max.z() ) \
                result.max.z() = v.z(); \
        }

        CHECK_VERTEX(vertices[faces[indices[i]].v1]);
        CHECK_VERTEX(vertices[faces[indices[i]].v2]);
        CHECK_VERTEX(vertices[faces[indices[i]].v3]);
#undef CHECK_VERTEX
    }

    return result;
}

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