//
// FILE:        MathUtil.h
// LICENSE:     The MIT license
// PURPOUSE:    Math utilities.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "Barycentric.h"
#include "shapes/Shapes.h"

namespace
{
    template < class T >
    T clamp ( const T& begin, const T& value, const T& end )
    {
        return ( (value < begin) ? begin : ((value > end) ? end : value ) );
    }

    bool intersect ( const Ray& ray, const Triangle& tri, Barycentric* collision )
    {
        return false;
    }
};
