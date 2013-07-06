//
// FILE:        Ray.h
// LICENSE:     The MIT license
// PURPOUSE:    Data structure for Rays in a 3D space.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../Vector.h"

struct Ray
{
    vec3f origin;
    vec3f delta;
};
