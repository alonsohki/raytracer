//
// FILE:        KDTree.h
// LICENSE:     The MIT license
// PURPOUSE:    KD-Tree based model space.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "AvgNormalCalculator.h"
#include <cassert>
#include "../Face.h"
#include "KDTree.h"
#include "../Vector.h"

using namespace ModelSpaces;

KDTree::KDTree ()
: mNormals(nullptr)
, mFaceNormals(nullptr)
{
}

KDTree::~KDTree ()
{
    delete [] mFaceNormals;
    delete [] mNormals;
}

void KDTree::load ( const vec3f* vertices, unsigned int vertexCount, const Face* faces, unsigned int faceCount )
{
    mFaces = faces;
    mFaceCount = faceCount;
    mVertices = vertices;
    mVertexCount = vertexCount;

    AvgNormalCalculator::calc ( mVertices, mVertexCount, mFaces, mFaceCount, &mNormals, &mFaceNormals );
    mBounds = BoundingBox::calculateFromVertices ( mVertices, mVertexCount );
}

void KDTree::getBounds ( BoundingBox* bbox ) const
{
    *bbox = mBounds;
}

bool KDTree::intersect ( const Ray& ray, Collision* c ) const
{
    return false;
}
