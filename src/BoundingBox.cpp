//
// FILE:        BoundingBox.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Bounding boxes
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "BoundingBox.h"
#include <float.h>
#include <limits>
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
    // From http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
    vec3f T_1, T_2; // vectors to hold the T-values for every direction
    double t_near = -DBL_MAX; // maximums defined in float.h
    double t_far = DBL_MAX;

    for (int i = 0; i < 3; i++){ //we test slabs in every direction
        if (ray.delta.v[i] == 0){ // ray parallel to planes in this direction
            if ((ray.origin.v[i] < min.v[i]) || (ray.origin.v[i] > max.v[i])) {
                return false; // parallel AND outside box : no intersection possible
            }
        } else { // ray not parallel to planes in this direction
            T_1.v[i] = (min.v[i] - ray.origin.v[i]) / ray.delta.v[i];
            T_2.v[i] = (max.v[i] - ray.origin.v[i]) / ray.delta.v[i];

            if(T_1.v[i] > T_2.v[i]){ // we want T_1 to hold values for intersection with near plane
                auto temp = T_1;
                T_1 = T_2;
                T_2 = temp;
            }
            if (T_1.v[i] > t_near){
                t_near = T_1.v[i];
            }
            if (T_2.v[i] < t_far){
                t_far = T_2.v[i];
            }
            if( (t_near > t_far) || (t_far < 0) ){
                return false;
            }
        }
    }
    *tmin = (float)t_near; *tmax = (float)t_far; // put return values in place
    return true; // if we made it here, there was an intersection - YAY
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
