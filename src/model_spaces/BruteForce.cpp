//
// FILE:        BruteForce.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Brute-force model space: No optimization at all in the collision detection.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "AvgNormalCalculator.h"
#include "BruteForce.h"
#include <cassert>
#include "../Face.h"
#include "../Vector.h"

using namespace ModelSpaces;

BruteForce::BruteForce ()
: mNormals(nullptr)
, mFaceNormals(nullptr)
{
}

BruteForce::~BruteForce ()
{
    delete [] mFaceNormals;
    delete [] mNormals;
}

void BruteForce::load ( const vec3f* vertices, unsigned int vertexCount, const Face* faces, unsigned int faceCount )
{
    mFaces = faces;
    mFaceCount = faceCount;
    mVertices = vertices;
    mVertexCount = vertexCount;

    AvgNormalCalculator::calc ( mVertices, mVertexCount, mFaces, mFaceCount, &mNormals, &mFaceNormals );
}

bool BruteForce::intersect ( const Ray& ray, Collision* c )
{
    vec3f v0 ( -1, 1, 1 );
    vec3f v1 ( 0, -1, 1 );
    vec3f v2 ( 1, 1, 1 );

    vec3f edge1 = v1 - v0;
    vec3f edge2 = v2 - v0;
    vec3f n = normalize(cross(edge2, edge1));

    c->t = std::numeric_limits<float>::infinity();

    // Compute gradient, which tells us how steep of an angle
    // we are approaching the front side of a triangle.
    float g = dot(n, ray.delta);


    // Is it paralel to the plane, or pointing from behind the front face?
    if ( !(g < 0.0f) )
        return false;


    // Compute the d value for the triangle plane equation.
    float d = dot(n, v0);


    // Compute the parametric point of intersection with the ray.
    // First calculate the enumerator to check for rejection if the
    // plane lies behind the ray start point.
    float t = d - dot(n, ray.origin);
    if ( !(t <= 0.0f) )
        return false;


    // Have we already found a closer intersection?
    if ( !(t >= g * c->t) )
        return false;


    // Here we know that the ray intersects the triangle _plane_.
    // Let's calculate the actual parametric value
    t /= g;
    assert(t >= 0.0f);
    assert(t <= c->t);
    c->t = t;


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
    if ( !(fabs(temp) > 0.000001f) )
        return false;
    temp = 1.0f / temp;

    // Compute the barycentric coordinates while checking for out of range.
    c->point.beta = (x0 * y2 - y0 * x2) * temp;
    if ( !(c->point.beta >= 0.0f) )
        return false;

    c->point.gamma = (x1 * y0 - y1 * x0) * temp;
    if ( !(c->point.gamma >= 0.0f) )
        return false;

    c->point.alpha = 1.0f - c->point.beta - c->point.gamma;
    if ( !(c->point.alpha >= 0.0f) )
        return false;

    c->point.normalize();

    // Copy the intersection triangle data
    c->v0.position = v0;
    c->v0.normal = n;
    c->v1.position = v1;
    c->v1.normal = n;
    c->v2.position = v2;
    c->v2.normal = n;

    return true;
}
