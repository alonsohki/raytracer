//
// FILE:        Barycentric.h
// LICENSE:     The MIT license
// PURPOUSE:    Barycentric coordinates
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "Barycentric.h"
#include "MathUtil.h"

void Barycentric::normalize ()
{
    // Make a fair normalization between the three coordinates
    v1 = clamp(0.0f, v1, 1.0f);
    v2 = clamp(0.0f, v2, 1.0f);
    v3 = clamp(0.0f, v3, 1.0f);

    // Normalize the length to 1
    float length = v1 + v2 + v3;
    if ( fabs(length) > 0.000001f )
    {
        v1 /= length;
        v2 /= length;
        v3 /= length;
    }

    // Make sure they sum 1
    float newV1 = 1.0f - v2 - v3;
    float newV2 = 1.0f - v1 - v3;
    float newV3 = 1.0f - v1 - v2;
    v1 = newV1;
    v2 = newV2;
    v3 = newV3;
}
