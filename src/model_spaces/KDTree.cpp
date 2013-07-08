//
// FILE:        KDTree.cpp
// LICENSE:     The MIT license
// PURPOUSE:    KD-Trees
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <algorithm>
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

KDTree::KDTree ()
: mRoot(nullptr)
{
}

KDTree::~KDTree ()
{
    if ( mRoot != nullptr )
    {
        std::vector<Node*> nodes;
        nodes.push_back ( mRoot );

        do
        {
            Node* top = nodes.back();
            nodes.pop_back();

            if ( !top->isLeaf() )
            {
                nodes.push_back(top->left);
                nodes.push_back(top->right);
            }

            delete top;
        }
        while ( nodes.size() > 0 );

        mRoot = nullptr;
    }
}

namespace
{
    void getStats ( Node* node, unsigned int depth, unsigned int* maxDepth, unsigned int* leafCount, unsigned int* maxPolys, float* averagePolys )
    {
        if ( depth > *maxDepth )
            *maxDepth = depth;

        if ( node->isLeaf() )
        {
            *averagePolys = ((*averagePolys) * (*leafCount) + node->indexCount) / ((*leafCount) + 1);
            (*leafCount)++;
            if ( node->indexCount > *maxPolys )
                *maxPolys = node->indexCount;
        }
        else
        {
            getStats ( node->left, depth + 1, maxDepth, leafCount, maxPolys, averagePolys );
            getStats ( node->right, depth + 1, maxDepth, leafCount, maxPolys, averagePolys );
        }
    }
}

void KDTree::buildFrom ( const vec3f* vertices, unsigned int vertexCount,
                         const Face* faces, unsigned int faceCount )
{
    // Build the faces data
    mFaceData.reserve ( faceCount );
    mIndices.reserve ( faceCount );
    for ( int i = 0; i < (int)faceCount; ++i )
    {
        const Face& cur = faces[i];
        FaceData data;
        data.centroid = (vertices[cur.v1] + vertices[cur.v2] + vertices[cur.v3]) / 3.0f;
        data.aabb = BoundingBox::calculateFromVertices(vertices, 3, cur.v1, cur.v2, cur.v3);
        mFaceData.push_back(data);

        mIndices.push_back(i);
    }

    BoundingBox rootAABB = BoundingBox::calculateFromVertices(vertices, vertexCount);

    BuildTreeContext context;
    context.vertices = vertices;
    context.vertexCount = vertexCount;
    context.faces = faces;
    context.faceCount = faceCount;
    mRoot = internalBuildFrom ( &context, rootAABB, &mIndices[0], mIndices.size(), 0 );

#if 1
    unsigned int maxDepth = 0;
    unsigned int leafCount = 0;
    unsigned int maxPolys = 0;
    float averagePolys = 0;
    getStats ( mRoot, 0, &maxDepth, &leafCount, &maxPolys, &averagePolys );
    printf("Max depth: %u, leaf count: %u, max polygons in a leaf: %u, average polys in a leaf: %f\n", maxDepth, leafCount, maxPolys, averagePolys);
#endif
}

Node* KDTree::internalBuildFrom ( void* context_, const BoundingBox& bounds, int* indices, unsigned int indexCount, int axis )
{
    BuildTreeContext* context = (BuildTreeContext *)context_;

    Node* node = new Node;
    node->aabb = bounds;
    node->splittingAxis = axis;

    // Stop conditions
    if ( indexCount < 2 )
    {
        node->indices = indices;
        node->indexCount = indexCount;
        node->left = nullptr;
        node->right = nullptr;
    }

    else
    {
        // Split by median
        std::sort(indices, indices + indexCount, [axis, this] ( int a, int b )
        {
            return mFaceData[a].centroid.v[axis] < mFaceData[b].centroid.v[axis];
        });
        FaceData& median = mFaceData[indexCount / 2];

        // Find the first polygon that doesn't lie in the left node
        unsigned int maxLeft;
        for ( maxLeft = indexCount / 2; maxLeft < indexCount; ++maxLeft )
        {
            if ( mFaceData[maxLeft].aabb.min.v[axis] > median.centroid.v[axis] )
                break;
        }

        // Find the first polygon that lies in the right node
        unsigned int minRight;
        for ( minRight = indexCount / 2; minRight > 0; --minRight )
        {
            if ( mFaceData[minRight].aabb.max.v[axis] < median.centroid.v[axis] )
                break;
        }
        ++minRight;

        // If any of the children contains all the elements, let's just transform this into a leaf
        if ( maxLeft == indexCount || minRight == 1 )
        {
            node->indices = indices;
            node->indexCount = indexCount;
            node->left = nullptr;
            node->right = nullptr;
        }
        else
        {
            BoundingBox leftBox = bounds;
            BoundingBox rightBox = bounds;

            leftBox.max.v[axis] = median.centroid.v[axis];
            rightBox.min.v[axis] = median.centroid.v[axis];

            node->indices = nullptr;
            node->indexCount = 0;
            node->left = internalBuildFrom ( context_, leftBox, &indices[0], maxLeft, ( axis + 1 ) % 3 );
            node->right = internalBuildFrom ( context_, rightBox, &indices[minRight], indexCount - minRight, ( axis + 1 ) % 3 );
        }
    }

    // Sort the indices based on the current axis
    return node;
}




Node* KDTree::findLeaf ( const vec3f& position, Node* node )
{
    if ( node == nullptr )
        return findLeaf ( position, getRoot() );

    if ( node->aabb.contains(position) == false )
        return nullptr;

    return internalFindLeaf(position, node);
}

Node* KDTree::internalFindLeaf ( const vec3f& position, Node* node )
{
    while ( !node->isLeaf() )
    {
        if ( node->left->aabb.max.v[node->splittingAxis] >= position.v[node->splittingAxis] )
            node = node->left;
        else
            node = node->right;
    }
    return node;
}
