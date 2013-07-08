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

    struct Node
    {
        int                 splittingAxis;
        BoundingBox         aabb;
        std::vector<int>    faces;
        Node*               left;
        Node*               right;

        bool isLeaf () const { return faces.size() == 0; }
        Node ()
        : left(nullptr)
        , right(nullptr)
        {
        }
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
        Node*       internalBuildFrom   ( void* context,
                                          int* indices, unsigned int indexCount,
                                          int axis );
        Node*       internalFindLeaf    ( const vec3f& position, Node* startAt );

    private:
        Node*       mRoot;
    };
} }
