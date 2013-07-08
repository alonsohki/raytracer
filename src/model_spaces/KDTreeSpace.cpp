//
// FILE:        KDTreeSpace.h
// LICENSE:     The MIT license
// PURPOUSE:    KD-Tree based model space.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "AvgNormalCalculator.h"
#include <cassert>
#include "../Face.h"
#include "KDTreeSpace.h"
#include "../Vector.h"

using namespace ModelSpaces;

KDTreeSpace::KDTreeSpace ()
: mNormals(nullptr)
, mFaceNormals(nullptr)
{
}

KDTreeSpace::~KDTreeSpace ()
{
    delete [] mFaceNormals;
    delete [] mNormals;
}

void KDTreeSpace::load ( const vec3f* vertices, unsigned int vertexCount, const Face* faces, unsigned int faceCount )
{
    mFaces = faces;
    mFaceCount = faceCount;
    mVertices = vertices;
    mVertexCount = vertexCount;

    AvgNormalCalculator::calc ( mVertices, mVertexCount, mFaces, mFaceCount, &mNormals, &mFaceNormals );
    mBounds = BoundingBox::calculateFromVertices ( mVertices, mVertexCount );

    mKDTree.buildFrom ( mVertices, mVertexCount, mFaces, mFaceCount );
}

void KDTreeSpace::getBounds ( BoundingBox* bbox ) const
{
    *bbox = mBounds;
}

bool KDTreeSpace::intersect ( const Ray& ray, Collision* c ) const
{
    return false;
}
