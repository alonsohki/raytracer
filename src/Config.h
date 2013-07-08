//
// FILE:        Config.h
// LICENSE:     The MIT license
// PURPOUSE:    App configuration.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#pragma once

#define MODEL_PATH "models/happy_vrip.ply"

// Bigger values of MAX_KDTREE_DEPTH and lower values of MIN_KDTREE_NODE_VOLUME
// speedup the rendering speed, but they penalize the KDtree construction times
// KDTREE_POOL_CHUNK_SIZE defines how many nodes will be allocated everytime that
// a block of nodes is allocated.
#define USE_KDTREE
#define MAX_KDTREE_DEPTH 32
#define MIN_KDTREE_NODE_VOLUME 0.0000001
#define KDTREE_POOL_CHUNK_SIZE 4096

#define TARGET_WIDTH 640
#define TARGET_HEIGHT 480
#define CLEAR_COLOR 0x000046
#define LIGHT_COLOR 0xE8D340

#define RAY_ORIGIN ( 0.25f, 0.25f, 1.0f )
#define RAY_DELTA ( -2.5f, -2.5f, -10.0f )
