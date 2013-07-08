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
        float                   splitPos;
        BoundingBox             aabb;
        std::vector<int>        indices;
#ifdef _DEBUG
        Node*                   parent;
#endif
        Node*                   left;
        Node*                   right;

        bool isLeaf () const { return left == nullptr; }
    };

    class KDTree
    {
    public:
                    KDTree              ();
        virtual     ~KDTree             ();

        Node*       getRoot             () const { return mRoot; }
        void        buildFrom           ( const vec3f* vertices, unsigned int vertexCount,
                                          const Face* faces, unsigned int faceCount );
        Node*       findLeaf            ( const vec3f& position, Node* startAt = nullptr ) const;

    private:
        void        internalBuildFrom   ( void* context, Node* node, int axis, unsigned int depth );
        Node*       internalFindLeaf    ( const vec3f& position, Node* startAt ) const;

    private:
        Node*                   mRoot;
        std::vector<FaceData>   mFaceData;
    };
} }
