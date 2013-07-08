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
    mKDTree.buildFrom ( mVertices, mVertexCount, mFaces, mFaceCount );
}

void KDTreeSpace::getBounds ( BoundingBox* bbox ) const
{
    if ( mKDTree.getRoot() )
        *bbox = mKDTree.getRoot()->aabb;
}

bool KDTreeSpace::intersect ( const Ray& ray_, Collision* c ) const
{
    // Traverse de KD-Tree
    bool intersected = false;
    const float epsilon = std::numeric_limits<float>::epsilon();
    Ray ray = ray_;

    // First find the intersection between the model bounds and the ray
    float tmin, tmax;
    BoundingBox bounds;
    getBounds ( &bounds );

    // If the ray doesn't intersect the AABB, we are pretty much done.
    if (!bounds.intersect(ray, &tmin, &tmax))
        return false;

    do
    {
        // Find the nearest leaf node for the intersection point
        vec3f point = ray.origin + ray.delta*(tmin + epsilon);
        KDTree::Node* node = mKDTree.findLeaf(point);
        if ( node == nullptr )
            return false;

        // Test against all the node elements
        for ( unsigned int i = 0; i < node->indices.size(); ++i )
        {
            const Face& face = mFaces[node->indices[i]];
            const vec3f& v0 = mVertices[face.v1];
            const vec3f& v1 = mVertices[face.v2];
            const vec3f& v2 = mVertices[face.v3];
            const vec3f& n = mFaceNormals[node->indices[i]];

            // Compute gradient, which tells us how steep of an angle
            // we are approaching the front side of a triangle.
            float g = dot(n, ray.delta);
        
            // Is it paralel to the plane, or pointing from behind the front face?
            if ( !(g < 0.0f) )
                continue;


            // Compute the d value for the triangle plane equation.
            float d = dot(n, v0);


            // Compute the parametric point of intersection with the ray.
            // First calculate the enumerator to check for rejection if the
            // plane lies behind the ray start point.
            float t = d - dot(n, ray.origin);
            if ( !(t <= 0.0f) )
                continue;


            // Have we already found a closer intersection?
            if ( !(fabs(t - g * c->t) > 0.000001f) )
                continue;


            // Here we know that the ray intersects the triangle _plane_.
            // Let's calculate the actual parametric value
            t /= g;
            assert(t >= 0.0f);

            // Computer the intersection 3D point
            vec3f p = ray.origin + ray.delta*t;
    
            // Project the triangle onto the most significant axis-aligned plane
            float x0, x1, x2;
            float y0, y1, y2;
            if ( fabs(n.x()) > fabs(n.y()) && fabs(n.x()) > fabs(n.z()) )
            {
                // Discard x, project onto yz
                x0 = p.y()  - v0.y();
                x1 = v1.y() - v0.y();
                x2 = v2.y() - v0.y();

                y0 = p.z()  - v0.z();
                y1 = v1.z() - v0.z();
                y2 = v2.z() - v0.z();
            }
            else if ( fabs(n.y()) > fabs(n.z()) )
            {
                // Discard y, project onto xz
                x0 = p.x()  - v0.x();
                x1 = v1.x() - v0.x();
                x2 = v2.x() - v0.x();

                y0 = p.z()  - v0.z();
                y1 = v1.z() - v0.z();
                y2 = v2.z() - v0.z();
            }
            else
            {
                // Discard z, project onto xy
                x0 = p.x() - v0.x();
                x1 = v1.x() - v0.x();
                x2 = v2.x() - v0.x();

                y0 = p.y()  - v0.y();
                y1 = v1.y() - v0.y();
                y2 = v2.y() - v0.y();
            }


            // Compute denominator and check for invalid polygon (empty area).
            float temp = x1 * y2 - y1 * x2;
            if ( !(temp != 0.0f) )
                continue;
            temp = 1.0f / temp;

            // Compute the barycentric coordinates while checking for out of range.
            Barycentric barycentric;
            barycentric.beta = (x0 * y2 - y0 * x2) * temp;
            if ( !(barycentric.beta >= 0.0f) )
                continue;

            barycentric.gamma = (x1 * y0 - y1 * x0) * temp;
            if ( !(barycentric.gamma >= 0.0f) )
                continue;

            barycentric.alpha = 1.0f - barycentric.beta - barycentric.gamma;
            if ( !(barycentric.alpha >= 0.0f) )
                continue;

            // Copy the intersection triangle data
            intersected = true;
            c->t = t;
            c->point = barycentric;
            c->point.normalize();
            c->v0.position = v0;
            c->v0.normal = mNormals[face.v1];
            c->v1.position = v1;
            c->v1.normal = mNormals[face.v2];
            c->v2.position = v2;
            c->v2.normal = mNormals[face.v3];
        }

        if (!intersected)
        {
            // Retry the intersection with a new ray
            ray.origin = ray.origin + ray.delta*(tmax + epsilon);
            if (!bounds.intersect(ray, &tmin, &tmax))
                return false;
        }
    }
    while ( !intersected );

    return intersected;
}
