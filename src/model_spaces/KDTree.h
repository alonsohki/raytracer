//
// FILE:        KDTree.h
// LICENSE:     The MIT license
// PURPOUSE:    KD-Trees
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "../BoundingBox.h"
#include "../Face.h"
#include <vector>
#include "../Vector.h"

namespace ModelSpaces { namespace KDTree {

    // Struct containing a face meta-data: centroid and aabb
    struct FaceData
    {
        vec3f           centroid;
        BoundingBox     aabb;
    };

    struct Node
    {
        int                     splittingAxis;
        BoundingBox             aabb;
        int*                    indices;
        unsigned int            indexCount;
        Node*                   left;
        Node*                   right;

        bool isLeaf () const { return indexCount != 0; }
    };

    class KDTree
    {
    public:
                    KDTree              ();
        virtual     ~KDTree             ();

        Node*       getRoot             () const { return mRoot; }
        void        buildFrom           ( const vec3f* vertices, unsigned int vertexCount,
                                          const Face* faces, unsigned int faceCount );
        Node*       findLeaf            ( const vec3f& position, Node* startAt = nullptr );

    private:
        Node*       internalBuildFrom   ( void* context, const BoundingBox& bounds, int* indices, unsigned int indexCount, int axis );
        Node*       internalFindLeaf    ( const vec3f& position, Node* startAt );

    private:
        Node*                   mRoot;
        std::vector<FaceData>   mFaceData;
        std::vector<int>        mIndices;
    };
} }
