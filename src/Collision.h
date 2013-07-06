//
// FILE:        Collision.h
// LICENSE:     The MIT license
// PURPOUSE:    Information about a collision.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "Barycentric.h"
#include "Vertex.h"

struct Collision
{
    // Barycentric coordinates of the hit in the polygon
    Barycentric point;

    // Hit polygon vertices
    Vertex v0;
    Vertex v1;
    Vertex v2;

    // Parametric collision in the ray
    float t;
};
