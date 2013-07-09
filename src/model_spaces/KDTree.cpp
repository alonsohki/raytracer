//
// FILE:        KDTree.cpp
// LICENSE:     The MIT license
// PURPOUSE:    KD-Trees
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <algorithm>
#include "../Config.h"
#include "KDTree.h"

using namespace ModelSpaces::KDTree;

//-----------------------------------------------
// Internal support structures for tree building
struct BuildTreeContext
{
    const vec3f* vertices;
    unsigned int vertexCount;
    const Face*  faces;
    unsigned int faceCount;
};

namespace
{
    void getStats ( Node* node, unsigned int depth, unsigned int* maxDepth, unsigned int* leafCount, unsigned int* maxPolys, float* averagePolys )
    {
        if ( depth > *maxDepth )
            *maxDepth = depth;

        if ( node->isLeaf() )
        {
            *averagePolys = ((*averagePolys) * (*leafCount) + node->indices.size()) / ((*leafCount) + 1);
            (*leafCount)++;
            if ( node->indices.size() > *maxPolys )
                *maxPolys = node->indices.size();
        }
        else
        {
            getStats ( node->left, depth + 1, maxDepth, leafCount, maxPolys, averagePolys );
            getStats ( node->right, depth + 1, maxDepth, leafCount, maxPolys, averagePolys );
        }
    }
}


KDTree::KDTree ()
: mRoot(nullptr)
, mNodePool(KDTREE_POOL_CHUNK_SIZE)
{
}

KDTree::~KDTree ()
{
}

void KDTree::buildFrom ( const vec3f* vertices, unsigned int vertexCount,
                         const Face* faces, unsigned int faceCount )
{
    // Build the faces data
    for ( int i = 0; i < (int)faceCount; ++i )
    {
        const Face& cur = faces[i];
        FaceData data;
        data.centroid = (vertices[cur.v1] + vertices[cur.v2] + vertices[cur.v3]) / 3.0f;
        data.aabb = BoundingBox::calculateFromVertices(vertices, 3, cur.v1, cur.v2, cur.v3);
        mFaceData.push_back(data);
    }

    BuildTreeContext context;
    context.vertices = vertices;
    context.vertexCount = vertexCount;
    context.faces = faces;
    context.faceCount = faceCount;

    mRoot = mNodePool.Alloc();
#ifdef _DEBUG
    mRoot->parent = nullptr;
#endif
    for ( int i = 0; i < (int)faceCount; ++i )
    {
        mRoot->indices.push_back(i);
    }
    mRoot->aabb = BoundingBox::calculateFromVertices(vertices, vertexCount);
    internalBuildFrom ( &context, mRoot, 0, 0 );

#ifdef _DEBUG
    unsigned int maxDepth = 0;
    unsigned int leafCount = 0;
    unsigned int maxPolys = 0;
    float averagePolys = 0;
    getStats ( mRoot, 0, &maxDepth, &leafCount, &maxPolys, &averagePolys );
    printf("Max depth: %u, leaf count: %u, max polygons in a leaf: %u, average polys in a leaf: %f\n", maxDepth, leafCount, maxPolys, averagePolys);
#endif
}

void KDTree::internalBuildFrom ( void* context_, Node* node, int axis, unsigned int depth )
{
    BuildTreeContext* context = (BuildTreeContext *)context_;

    node->splittingAxis = axis;

    // Stop if we got only one polygon in this node, the node volume is too small or we reached the max allowed depth
    if ( node->indices.size() < 2 || node->aabb.volume() < MIN_KDTREE_NODE_VOLUME || depth >= MAX_KDTREE_DEPTH )
    {
        node->left = nullptr;
        node->right = nullptr;
        return;
    }

    // Split by median
    std::sort(node->indices.begin(), node->indices.end(), [axis, this] ( int a, int b )
    {
        return mFaceData[a].centroid.v[axis] < mFaceData[b].centroid.v[axis];
    });

    unsigned int medianPos = node->indices.size() / 2;
    FaceData& median = mFaceData[node->indices[medianPos]];
    float splitPos = median.centroid.v[axis];
    node->splitPos = splitPos;

    // Create the left and right nodes
    node->left = mNodePool.Alloc();
    node->right = mNodePool.Alloc();

#ifdef _DEBUG
    node->left->parent = node;
    node->right->parent = node;
#endif

    // Split the polygons by the newly created nodes
    for ( auto& index : node->indices )
    {
        auto& cur = mFaceData[index];
        if ( cur.aabb.min.v[axis] <= (splitPos+0.0001f) )
            node->left->indices.push_back(index);
        if ( cur.aabb.max.v[axis] >= (splitPos-0.0001f) )
            node->right->indices.push_back(index);
    }

    // If any of the child nodes contains all the indices, do not expand anymore
    if ( node->left->indices.size() == node->indices.size() ||
         node->right->indices.size() == node->indices.size() )
    {
        mNodePool.Free(node->left);
        mNodePool.Free(node->right);
        node->left = nullptr;
        node->right = nullptr;
        return;
    }

    // Subdivide the child nodes
    // Do not always distribute it by threads or the thread creation overhead will
    // make it expensive. Just once out of four.
    if ( (depth % 4) == 0 )
    {
#pragma omp parallel sections
    {
#pragma omp section
        {
            node->left->aabb = node->aabb;
            node->left->aabb.max.v[axis] = splitPos;
            internalBuildFrom ( context_, node->left, ( axis + 1 ) % 3, depth + 1 );
        }
#pragma omp section
        {
            node->right->aabb = node->aabb;
            node->right->aabb.min.v[axis] = splitPos;
            internalBuildFrom ( context_, node->right, ( axis + 1 ) % 3, depth + 1 );
        }
    }
    }
    else
    {
        {
            node->left->aabb = node->aabb;
            node->left->aabb.max.v[axis] = splitPos;
            internalBuildFrom ( context_, node->left, ( axis + 1 ) % 3, depth + 1 );

            node->right->aabb = node->aabb;
            node->right->aabb.min.v[axis] = splitPos;
            internalBuildFrom ( context_, node->right, ( axis + 1 ) % 3, depth + 1 );
        }
    }
}




Node* KDTree::findLeaf ( const vec3f& position, Node* node ) const
{
    if ( node == nullptr )
        return findLeaf ( position, getRoot() );

    if ( node->aabb.contains(position) == false )
        return nullptr;

    return internalFindLeaf(position, node);
}

Node* KDTree::internalFindLeaf ( const vec3f& position, Node* node ) const
{
    while ( !node->isLeaf() )
    {
        if ( node->splitPos >= position.v[node->splittingAxis] )
            node = node->left;
        else
            node = node->right;
    }
    return node;
}
