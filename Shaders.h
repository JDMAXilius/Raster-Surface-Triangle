#pragma once
#include "Mymath.h"

//// The active vertex shader. Modifies an incoming vertex. Pre-Rasterization. 
void (*VertexShader)(VERTEX&) = 0;
//// The active pixel shader. Modifies an outgoing pixel. Post-Rasterization.
void (*PixelShader)(unsigned int&) = 0;
//
//// All Shader Variables (Always Pre-fixed by “SV_”)
mat3x3 SV_WorldMatrix;

// Various custom vertex and pixel shaders, (Pre-fixed by “VS_” & “PS_”)
// Can be swapped using above function pointers as needed for flexibility. 

// Applys the current world matrix to all
void VS_World(VERTEX& multiplyMe)
{
    multiplyMe = vertexMultMatrix(multiplyMe, SV_WorldMatrix);

}
// Basic pixel shader returns the color white
void PS_White(unsigned int& makeWhite)
{
    makeWhite = 0xFFFFFFFF;
}