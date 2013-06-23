//
// FILE:        BruteForce.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Brute-force model space: No optimization at all in the collision detection.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "BruteForce.h"

using namespace ModelSpaces;

BruteForce::BruteForce ()
{
}

BruteForce::~BruteForce ()
{
}

void BruteForce::load ( const float* vertices, unsigned int vertexCount, const int* faces, unsigned int faceCount )
{
}

bool BruteForce::intersect ( const Ray& ray, vec3f* pos, vec3f* normal )
{
    return false;
}
