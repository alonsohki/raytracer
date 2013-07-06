//
// FILE:        BruteForce.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Brute-force model space: No optimization at all in the collision detection.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "AvgNormalCalculator.h"
#include "BruteForce.h"
#include "../Face.h"
#include "../Vector.h"

using namespace ModelSpaces;

BruteForce::BruteForce ()
: mNormals(nullptr)
{
}

BruteForce::~BruteForce ()
{
    delete [] mNormals;
}

void BruteForce::load ( const vec3f* vertices, unsigned int vertexCount, const Face* faces, unsigned int faceCount )
{
    mFaces = faces;
    mFaceCount = faceCount;
    mVertices = vertices;
    mVertexCount = vertexCount;

    AvgNormalCalculator::calc ( mVertices, mVertexCount, mFaces, mFaceCount, &mNormals );
}

bool BruteForce::intersect ( const Ray& ray, vec3f* pos, vec3f* normal )
{
    return false;
}
