#include <iostream>
#include <assert.h>
#include "RasterSurface.h"
#include <Windows.h>
#include "rasterizer.h"
#include "XTime.h"
#include <vector>
using namespace std;

int main()
{
	RS_Initialize(Raster_W, Raster_H);

	//time
	XTime time;
	// Set up rotation matrices
	mat3x3 matRotZ, matRotX;
	// Initialize constant drawing data. (BE SURE IT IS CONSTANT!!!)
	const VERTEX UnEditableOriginalLine[3] =
	{
		 {-0.5, +0.5, 1, 1, colorR }, { +0.5, -0.5, 1, 1, colorG }, { -0.5, -0.5, 1, 1, colorB }  // Defines a fixed diagonal line crossing the origin.    
	};
	const triangleV verticesOriginals =
	{
		{-0.5, +0.5, 1, 1, colorR , +0.5, -0.5, 1, 1, colorG , -0.5, -0.5, 1, 1, colorB }
	};

	do {

		Clear(0x000000);
		//RUNS TIME
		time.Signal();

		// Select appropriate Shaders you want to Use. REMEMBER! You can make as many as you want!
		// Only ever having one type of shader defeats the purpose! (This exists for flexibility!)
		VertexShader = VS_World; // I choose a shader that will multiply by a single matrix that I control.  
		PixelShader = PS_White; // Regardless of the original color I want all pixels to be shown white. 
		
		// Set any custom variables used by the shader functions so you can control shader behaviors directly! 
		SV_WorldMatrix = matrixRotationZ(time.TotalTimeExact() * fTheta);
		
		//Fill Triangule
		fillTriangle(UnEditableOriginalLine[0], UnEditableOriginalLine[1], UnEditableOriginalLine[2], colorG);
		
		//Draw triangule lines
		// With appropriate shaders chosen we draw the fixed line. (A copy will be modifed by shaders)
		DrawLine(UnEditableOriginalLine[0], UnEditableOriginalLine[1]);
		DrawLine(UnEditableOriginalLine[1], UnEditableOriginalLine[2]);
		DrawLine(UnEditableOriginalLine[2], UnEditableOriginalLine[0]);

	} while (RS_Update(raster, R_PIXELS));
	

	RS_Shutdown();
	return 0;

}