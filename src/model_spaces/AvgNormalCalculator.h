//
// FILE:        AvgNormalCalculator.h
// LICENSE:     The MIT license
// PURPOUSE:    Calculate the normals of a model using the average.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "../Face.h"
#include "../Vector.h"

namespace ModelSpaces
{
    class AvgNormalCalculator
    {
    public:
        static void calc ( const vec3f* vertices, unsigned int vertexCount,
                           const Face* faces, unsigned int faceCount,
                           vec3f** normals );
    };
}
