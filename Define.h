#pragma once
#include <vector>
#include "RasterSurface.h"
using namespace std;

// define.h
#define Raster_W 500 
#define Raster_H 500 
#define R_PIXELS ((Raster_W)*(Raster_H))
unsigned int raster[R_PIXELS] = { 0, };
unsigned colorR = 0xFF0000;
unsigned colorG = 0x00FF00;
unsigned colorB = 0x0000FF;

float fTheta = -60;

struct VERTEX
{
	float x,y,z,w;
	unsigned int color;
};

struct triangleV
{
	VERTEX p[3];
};
struct meshV
{
	vector<triangleV> trisV;
};
struct mat3x3
{
	float m[3][3] = { 0 };
};
//
struct vec3d
{
	float x, y, z;
};

struct triangle
{
	vec3d p[3];
};

struct mesh
{
	vector<triangle> tris;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};