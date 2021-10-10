#pragma once
#define _USE_MATH_DEFINES
#include "Define.h"

using namespace std;

// mymath.h

// Ax + By + C = 0
//(Y1 - Y2)x + (X2 - X1)y + X1Y2 - Y1X2 = 0
float ImplicitLineEq2(VERTEX a, VERTEX b, VERTEX p)
{
	return (a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - a.y * b.x;
}

//lerp(a, b, x) = a + (b -a ) * x
float lerpEq(int a, int b, int r)
{
	return(b - a) * r + a;
}

//lerp(a, b, c, x) = ((a * x) + (b * x) + (c * x)); or P=(uA)+(vB)+(wC)
float barycentriclerpEq(int a, int b, int c, VERTEX r)
{
	return((a * r.x) + (b * r.y) + (c * r.z));
	
}

void NDCToScreen(float ndc_x, float ndc_y)
{
	//NDC -> Screen
	int screen_x, screen_y;
	screen_x = (ndc_x + 1) * ((Raster_W - 1) / 2);
	screen_y = (1 - ndc_y) * ((Raster_H - 1) / 2);
}

VERTEX NDC2(VERTEX ndc)
{
	//NDC -> Screen
	VERTEX screen;
	
	screen.x = ((ndc.x + 1) * (Raster_W / 2));
	screen.y = ((1 - ndc.y) * (Raster_H / 2));
	//result
	return screen;
}
void drawpixel(int x, int y, int W, unsigned int color)
{
	raster[W * y + x] = color;
}

//Barycentric Interpolation
VERTEX BarycentricCoordinates(VERTEX a, VERTEX b, VERTEX c, VERTEX p)
{
	VERTEX Pβγα;

	float β = ImplicitLineEq2(b, a, c);
	float γ = ImplicitLineEq2(c, b, a);
	float α = ImplicitLineEq2(a, c, b);
	float B = ImplicitLineEq2(p, a, c);
	float Y = ImplicitLineEq2(p, b, a);
	float A = ImplicitLineEq2(p, c, b);

	Pβγα = { (B / β), (Y / γ), (A / α) };

	return Pβγα;
}

triangleV triangleVMultMatrix(triangleV v, mat3x3 m)
{
	triangleV t;
	t.p[0].x = (v.p[0].x * m.m[0][0]) + (v.p[0].y * m.m[1][0]) + (v.p[0].z * m.m[2][0]);
	t.p[1].x = (v.p[1].x * m.m[0][1]) + (v.p[1].y * m.m[1][1]) + (v.p[1].z * m.m[2][1]);
	t.p[2].x = (v.p[2].x * m.m[0][2]) + (v.p[2].y * m.m[1][2]) + (v.p[2].z * m.m[2][2]);
	float w = v.p[0].x * m.m[0][2] + v.p[0].y * m.m[1][2] + v.p[0].y * m.m[2][2];

	if (w != 0.0f)
	{
		t.p[0].x /= w; t.p[0].y /= w; t.p[0].z /= w;
	}
	return t;
}

VERTEX vertexMultMatrix(VERTEX v, mat3x3 m)
{
	VERTEX vex;
	vex.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
	vex.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
	vex.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);
	float w = v.x * m.m[0][2] + v.y * m.m[1][2] + v.y * m.m[2][2];
	return vex;
}

mat3x3 matrixMultMatrix(mat3x3 m, mat3x3 l)
{
	mat3x3 results;
	float product = 0;
	int i;
	int j;

	for (i = 1; i <= 3; i++) {
		for (j = 1; j <= 3; j++) {
			product += m.m[i][j] * l.m[j][i];
			cout << product << endl;
		}
		results.m[i][j] = product;
		product = 0;
	}
	return results;
}
void matrixIdentity()
{

}
mat3x3 matrixRotationZ( float fTheta)
{
	mat3x3 matRotX = { 0 };
	fTheta = fTheta / 180.0f * 3.14;

	// Rotation Z
	matRotX.m[0][0] = cosf(fTheta * 0.5f);
	matRotX.m[0][1] = sinf(fTheta * 0.5f);
	matRotX.m[1][0] = -sinf(fTheta * 0.5f);
	matRotX.m[1][1] = cosf(fTheta * 0.5f);
	matRotX.m[2][2] = 1;

	return matRotX;
}
mat3x3 matrixRotationX( float fTheta)
{
	mat3x3 matRotX = { 0 };
	fTheta = fTheta / 180.0f * 3.14;
	
	// Rotation X
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(fTheta * 0.5f);
	matRotX.m[1][2] = sinf(fTheta * 0.5f);
	matRotX.m[2][1] = -sinf(fTheta * 0.5f);
	matRotX.m[2][2] = cosf(fTheta * 0.5f);
	return matRotX;
}
void matrixTranslation()
{

}

void MultiplyMatrixVector(triangleV v, triangleV o, mat3x3 m)
{
	
	o.p[0].x = (v.p[0].x * m.m[0][0]) + (v.p[0].y * m.m[1][0]) + (v.p[0].z * m.m[3][0]);
	o.p[1].x = (v.p[1].x * m.m[0][1]) + (v.p[1].y * m.m[1][1]) + (v.p[1].z * m.m[3][1]);
	o.p[2].x = (v.p[2].x * m.m[0][2]) + (v.p[2].y * m.m[1][2]) + (v.p[2].z * m.m[3][2]);
	float w = v.p[0].x * m.m[0][3] + v.p[0].y * m.m[1][3] + v.p[0].y * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.p[0].x /= w; o.p[0].y /= w; o.p[0].z /= w;
	}
}

