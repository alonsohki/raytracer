//
// FILE:        Barycentric.h
// LICENSE:     The MIT license
// PURPOUSE:    Barycentric coordinates
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

struct Barycentric
{
    float alpha;
    float beta;
    float gamma;

    Barycentric ()
    {
    }

    Barycentric ( const float& a, const float& b, const float& c )
    {
        alpha = a;
        beta = b;
        gamma = c;

        normalize ();
    }

    void normalize ();
};
