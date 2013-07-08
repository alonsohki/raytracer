//
// FILE:        Renderer.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Raytracing renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "Config.h"
#include "MathUtil.h"
#include "tga_saver.h"
#include "Renderer.h"

void Renderer::renderModel ( PixBuffer* target, IModelSpace* model ) const
{
    BoundingBox bounds;

    // Calculate the X and Y steps
    model->getBounds ( &bounds );
    float width = bounds.width();
    float height = bounds.height();
    float left;
    float top;
    float stepX;
    float stepY;

    if ( height > width )
    {
        top = bounds.max.y();
        stepY = height / target->getHeight();
        stepX = stepY;
        left = (bounds.min.x() + width*0.5f) - (stepX * target->getWidth())*0.5f;
    }
    else
    {
        left = bounds.min.x();
        stepX = width / target->getWidth();
        stepY = stepX;
        top = (bounds.max.y() - height*0.5f) + (stepY * target->getHeight())*0.5f;
    }


    // Scan the model
    int x = 0;
    int i, j;
    const vec3f rayDelta RAY_DELTA;
    const vec3f lightDir = normalize(rayDelta);

    #pragma omp parallel for
    for ( j = 0; j < (int)target->getHeight(); ++j )
    {
        for ( i = 0; i < (int)target->getWidth(); ++i )
        {
            Collision col;
            Ray ray;
            ray.delta = rayDelta;
            ray.origin = vec3f(left + stepX * i, top - stepY * j, 0.0f) + vec3f RAY_ORIGIN;
            if ( model->intersect(ray, &col) == true )
            {
                vec3f normal = col.v0.normal * col.point.alpha + col.v1.normal * col.point.beta + col.v2.normal * col.point.gamma;
                float diffuse = saturate(-dot(normal, lightDir));
                unsigned int color = (unsigned int)( ((LIGHT_COLOR >> 16) & 0xFF) * diffuse ) << 16 |
                                     (unsigned int)( ((LIGHT_COLOR >> 8) & 0xFF) * diffuse ) << 8 |
                                     (unsigned int)( ((LIGHT_COLOR >> 0) & 0xFF) * diffuse ) << 0 |
                                     0xFF000000;
                target->setPixel(i, j, color);
            }
        }
    }
}
