//
// FILE:        KDTree.cpp
// LICENSE:     The MIT license
// PURPOUSE:    KD-Trees
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "KDTree.h"

using namespace ModelSpaces::KDTree;

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

void KDTree::buildFrom ( const vec3f* vertices, unsigned int vertexCount,
                         const Face* faces, unsigned int faceCount )
{
    std::vector<int> indices ( faceCount );
    for ( int i = 0; i < (int)faceCount; ++i )
        indices.push_back(i);

    BuildTreeContext context;
    context.vertices = vertices;
    context.vertexCount = vertexCount;
    context.faces = faces;
    context.faceCount = faceCount;
    mRoot = internalBuildFrom ( &context, &indices[0], indices.size(), 0 );
}

Node* KDTree::internalBuildFrom ( void* context_,
                                  int* indices, unsigned int indexCount,
                                  int axis )
{
    BuildTreeContext* context = (BuildTreeContext *)context_;
    return nullptr;
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
