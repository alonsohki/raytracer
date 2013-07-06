//
// FILE:        Renderer.h
// LICENSE:     The MIT license
// PURPOUSE:    Raytracing renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#include "model_spaces/IModelSpace.h"
#include "PixBuffer.h"

class Renderer
{
public:
    void        renderModel     ( PixBuffer* target, IModelSpace* model ) const;
};
