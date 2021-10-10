#pragma once
#include "Shaders.h"

//rasterizer.h
void Clear(unsigned int color)
{
	for (size_t i = 0; i < R_PIXELS; i++)
	{
		raster[i] = color;
	}
}

void PlotPixel(int x, int y, unsigned color)
{
	assert(x >= 0 && x <= Raster_W);
	assert(y >= 0 && y <= Raster_H);
	raster[Raster_W * y + x] = color;
}

void DrawLinepixel(int x1, int y1, int x2, int y2, unsigned color)
{

	unsigned colorS = color;

	int deltaXB = std::abs(y1 - x1);
	int deltaYB = std::abs(y2 - x2);
	int totalPB = (deltaXB > deltaYB) ? deltaXB : deltaYB;

	for (int i = 0; i < totalPB; i++)
	{
		float rB = static_cast<float>(i) / static_cast<float>(totalPB);
		// (B-A) * R + A
		/*int xB = (y1 - x1) * rB + x1;
		int yB = (y2 - x2) * rB + x2;*/
		int xB = (x1 + rB *(y1 - x1) + 0.5f);
		int yB = (x2 + rB *(y2 - x2) + 0.5f);

		PlotPixel(xB, yB, colorS);
	}

	////End points Bresenham
	PlotPixel(x1, x2, 0xFFFF00);
	PlotPixel(y1, y2, 0xFFFF00);
}
unsigned ColorLerp(unsigned a, unsigned b, int r)
{
	//a values
	int a1 = (a & 0xFF000000) >> 24;
	int a2 = (a & 0x00FF0000) >> 16;
	int a3 = (a & 0x0000FF00) >> 8;
	int a4 = (a & 0x000000FF);

	//b values
	int b1 = (b & 0xFF000000) >> 24;
	int b2 = (b & 0x00FF0000) >> 16;
	int b3 = (b & 0x0000FF00) >> 8;
	int b4 = (b & 0x000000FF);

	//math of lerp
	unsigned int f1 = (b1 - a1) * r + a1;
	unsigned int f2 = (b2 - a2) * r + a2;
	unsigned int f3 = (b3 - a3) * r + a3;
	unsigned int f4 = (b4 - a4) * r + a4;

	return (f1 << 24) | (f2 << 16) | (f3 << 8) | f4;
}

unsigned BarycentricCoordinatesColorLerp(unsigned a, unsigned b, unsigned c, VERTEX r)
{
	//a values
	int a1 = (a & 0xFF000000) >> 24;
	int a2 = (a & 0x00FF0000) >> 16;
	int a3 = (a & 0x0000FF00) >> 8;
	int a4 = (a & 0x000000FF);

	//b values
	int b1 = (b & 0xFF000000) >> 24;
	int b2 = (b & 0x00FF0000) >> 16;
	int b3 = (b & 0x0000FF00) >> 8;
	int b4 = (b & 0x000000FF);

	//c values
	int c1 = (c & 0xFF000000) >> 24;
	int c2 = (c & 0x00FF0000) >> 16;
	int c3 = (c & 0x0000FF00) >> 8;
	int c4 = (c & 0x000000FF);

	//math of lerp
	unsigned int f1 = barycentriclerpEq(a1, b1, c1, r);
	unsigned int f2 = barycentriclerpEq(a2, b2, c2, r);
	unsigned int f3 = barycentriclerpEq(a3, b3, c3, r);
	unsigned int f4 = barycentriclerpEq(a4, b4, c4, r);

	return (f1 << 24) | (f2 << 16) | (f3 << 8) | f4;
}

void DrawLinepixelvertex(VERTEX a, VERTEX b, unsigned int color)
{

	unsigned colorS = color;

	int deltaXB = std::abs(b.x - a.x);
	int deltaYB = std::abs(b.y - a.y);
	int totalPB = (deltaXB > deltaYB) ? deltaXB : deltaYB;

	for (int i = 0; i < totalPB; i++)
	{
		float rB = i / static_cast<float>(totalPB);
		// (B-A) * R + A
		/*int xB = (y1 - x1) * rB + x1;
		int yB = (y2 - x2) * rB + x2;*/
		int xB = (a.x + rB * (b.x - a.x) + 0.5f);
		int yB = (a.y + rB * (b.y - a.y) + 0.5f);

		PlotPixel(xB, yB, colorS);
	}

	//End points Bresenham
	PlotPixel(a.x, a.y, 0xFFFF00);
	PlotPixel(b.x, b.y, 0xFFFF00);
}


void DrawTriangle3(VERTEX a, VERTEX b, VERTEX c)
{
	//points
	VERTEX A = { a };
	VERTEX B = { b };
	VERTEX C = { c };

	//NDC -> Screen
	a = NDC2(A);
	b = NDC2(B);
	c = NDC2(C);

	//Drawing pixels & lines
	DrawLinepixelvertex(a, b, 0xFFFFFFFF);
	DrawLinepixelvertex(b, c, 0xFFFFFFFF);
	DrawLinepixelvertex(c, a, 0xFFFFFFFF);
}
void DrawTriangle2(VERTEX a, VERTEX b, VERTEX c)
{
	//points
	VERTEX A = { a };
	VERTEX B = { b };
	VERTEX C = { c };

	//NDC -> Screen
	a = NDC2(A);
	b = NDC2(B);
	c = NDC2(C);

	//Drawing pixels & lines
	DrawLinepixel(a.x, a.y, b.x, b.y, 0xFFFFFFFF);
	DrawLinepixel(b.x, b.y, c.x, c.y, 0xFFFFFFFF);
	DrawLinepixel(c.x, c.y, a.x, a.y, 0xFFFFFFFF);
}

void fillTriangle(VERTEX a, VERTEX b, VERTEX c, unsigned color)
{

	//DrawTriangle3(a, b, c);
	VERTEX A = { a };
	VERTEX B = { b };
	VERTEX C = { c };

	// Use vertex shader to modify incoming copies only.
	if (VertexShader)
	{
		VertexShader(A);
		VertexShader(B);
		VertexShader(C);
	}

	//NDC -> Screen
	a = NDC2(A);
	b = NDC2(B);
	c = NDC2(C);

	//loop across screen, examine x & y
	for (int y = 0; y < Raster_H; y++)
	{
		for (int x = 0; x < Raster_W; x++)
		{
			//x&y pixel loc
			VERTEX P
			{
				static_cast<float>(x),
				static_cast<float>(y),
			};
			//the three pionts
			//the three joining lines
			float result1 = ImplicitLineEq2(a, b, P);
			float result2 = ImplicitLineEq2(b, c, P);
			float result3 = ImplicitLineEq2(a, c, P);

			color = BarycentricCoordinatesColorLerp(colorR, colorG, colorB, 
				BarycentricCoordinates(a, b, c, P));

			if (result1 > 0 && result2 > 0 && result3 < 0)
				raster[y * Raster_W + x] = color;
		}
	}
}

//// Draws a line using one of the line equations.
void DrawLine(const VERTEX& start, const VERTEX& end)
{
	// Copy input data and send through shaders
	VERTEX copy_start = start;
	VERTEX copy_end = end;
	// Use vertex shader to modify incoming copies only.
	if (VertexShader)
	{
		VertexShader(copy_start);
		VertexShader(copy_end);
	}
	// original plotting variables adapted to use new cartesian data
	VERTEX screen_start = NDC2(copy_start);
	VERTEX screen_end = NDC2(copy_end);
	// Standard line drawing code follows using integer coordinates...
	DrawLinepixelvertex(screen_start, screen_end, 0xFFFFFFFF);
}