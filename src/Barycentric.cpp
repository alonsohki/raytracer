//
// FILE:        Barycentric.h
// LICENSE:     The MIT license
// PURPOUSE:    Barycentric coordinates
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include "Barycentric.h"
#include <cmath>
#include "MathUtil.h"

void Barycentric::normalize ()
{
    // Make a fair normalization between the three coordinates
    alpha = clamp(0.0f, alpha, 1.0f);
    beta  = clamp(0.0f, beta,  1.0f);
    gamma = clamp(0.0f, gamma, 1.0f);

    // Normalize the length to 1
    float length = alpha + beta + gamma;
    if ( fabs(length) > 0.000001f )
    {
        alpha /= length;
        beta  /= length;
        gamma /= length;
    }

    // Make sure they sum 1
    float newAlpha = 1.0f - beta - gamma;
    float newBeta = 1.0f - alpha - gamma;
    float newGamma = 1.0f - alpha - beta;
    alpha = newAlpha;
    beta = newBeta;
    gamma = newGamma;
}
