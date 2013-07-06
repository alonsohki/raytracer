//
// FILE:        Barycentric.h
// LICENSE:     The MIT license
// PURPOUSE:    Barycentric coordinates
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

struct Barycentric
{
    float v1;
    float v2;
    float v3;

    Barycentric ( const float& v1, const float& v2, const float& v3 )
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;

        normalize ();
    }

    void normalize ();
};
