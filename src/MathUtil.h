//
// FILE:        MathUtil.h
// LICENSE:     The MIT license
// PURPOUSE:    Math utilities.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

namespace
{
    template < class T >
    T clamp ( const T& begin, const T& value, const T& end )
    {
        return ( (value < begin) ? begin : ((value > end) ? end : value ) );
    }

    template < class T >
    T lerp ( const T& from, const T& to, float delta )
    {
        return from + ( to - from ) * delta;
    }
};
