//
// FILE:        Renderer.cpp
// LICENSE:     The MIT license
// PURPOUSE:    Raytracing renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "MathUtil.h"
#include "tga_saver.h"
#include "Renderer.h"

void Renderer::renderModel ( PixBuffer* target, IModelSpace* model ) const
{
    BoundingBox bounds;

    // Calculate the X and Y steps
    model->getBounds ( &bounds );
    float width = bounds.max.x() - bounds.min.x();
    float height = bounds.max.y() - bounds.min.y();
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
    int i, j;
    for ( j = 0; j < (int)target->getHeight(); ++j )
    {
        #pragma omp parallel for
        for ( i = 0; i < (int)target->getWidth(); ++i )
        {
            Collision col;
            Ray ray;
            ray.delta = vec3f(0.0f, 0.0f, 10.0f);
            ray.origin = vec3f(left + stepX * i, top - stepY * j, -1.0f);
            if ( model->intersect(ray, &col) == true )
            {
                vec3f normal = col.v0.normal * col.point.alpha + col.v1.normal * col.point.beta + col.v2.normal * col.point.gamma;
                vec3f lightDir = normalize(ray.delta);
                float diffuse = saturate(-dot(normal, lightDir));
                unsigned int color = (unsigned int)(0xFF * diffuse) & 0xFF;
                color |= color << 8 | color << 16 | color << 24;
                target->setPixel(i, j, color);
            }
        }

            PixBuffer result ( target->getWidth(), target->getHeight(), target->getDepth() );
    for ( unsigned int a = 0; a < target->getHeight(); ++a )
        for ( unsigned int b = 0; b < target->getWidth(); ++b )
            result.setPixel(b, target->getHeight() - a - 1, target->getPixel(b, a));
    Write_Tga("output/result.tga", result.getWidth(), result.getHeight(), result.getBuffer());
    }
}