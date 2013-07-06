//
// FILE:        AvgNormalCalculator.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Calculate the normals of a model using the average.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "AvgNormalCalculator.h"
#include "../Vector.h"

using namespace ModelSpaces;

struct AvgMeta
{
    unsigned int faceCount;
};

void AvgNormalCalculator::calc ( const vec3f* vertices, unsigned int vertexCount,
                                 const Face* faces, unsigned int faceCount,
                                 vec3f** normals )
{
    // Allocate as many normals as vertices
    *normals = new vec3f [ vertexCount ];
    memset ( *normals, 0, sizeof(vec3f) * vertexCount );

    // Allocate the metadata for our average calculator
    AvgMeta* meta = new AvgMeta [ vertexCount ];
    memset ( meta, 0, sizeof(AvgMeta) * vertexCount );

    // Go through every face and calculate the polygon normal. Then, accumulate it
    // onto the normals buffer.
    for ( unsigned int i = 0; i < faceCount; ++i )
    {
        const Face& face = faces[i];
        const vec3f& v1 = vertices[face.v1];
        const vec3f& v2 = vertices[face.v2];
        const vec3f& v3 = vertices[face.v3];

        // Get the vectors of the first and second polygon edges.
        vec3f dir1 = v2 - v1;
        vec3f dir2 = v3 - v1;

        // Calculate the normal with the cross product of the edge vectors
        // Ignore empty polygons
        try
        {
            vec3f normal = normalize(cross(dir1, dir2));

            // Accumulate this normal onto all the polygon vertices
            meta[face.v1].faceCount++;
            meta[face.v2].faceCount++;
            meta[face.v3].faceCount++;
            (*normals)[face.v1] += normal;
            (*normals)[face.v2] += normal;
            (*normals)[face.v3] += normal;
        }
        catch ( ... ) {}
    }

    // Calculate the averages
    for ( unsigned int i = 0; i < vertexCount; ++i )
    {
        if ( meta[i].faceCount > 0 )
            (*normals)[i] /= (float)(meta[i].faceCount);
    }

    // Delete the temporary metadata
    delete [] meta;
}
