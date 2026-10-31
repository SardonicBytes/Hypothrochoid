#include <raylib.h>
#include <raymath.h>
#include "DuckTools.h"
#include <vector>

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 1000

int R = 400; // outer radius
int r = 300; // inner radius
Color color = GREEN; // colour of drawing
Vector2 drawOffset = Vector2{0,250}; // offsets of points from small circle at alpha 0

int main()
{
	InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Chaos!");

	float alpha = 0;
	float innerRotation = 0;
	Vector2 Cen = Vector2{ WINDOW_SIZE_X / 2,WINDOW_SIZE_Y / 2 };
	Vector2 cen;

	float deltaTime;
	float moveSpeed = 200;
	float innerRotateSpeed = 50;

	std::vector<Vector2> drawPoints;
	Vector2 drawPoint = { (float)r, 0 };

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		deltaTime = GetFrameTime();
		cen = Cen + Vector2{PolarToVector(R-r,alpha)};

		DrawCircleLines(Cen.x,Cen.y,R,color);
		DrawCircleLines(cen.x,cen.y,r,color);

		alpha += deltaTime * moveSpeed;
		innerRotation += deltaTime * innerRotateSpeed;

		drawPoint = cen + Vector2Rotate(drawOffset,innerRotation);
		drawPoints.push_back(drawPoint);

		DrawLineStrip(drawPoints.data(), drawPoints.size(),color);

		EndDrawing();
	}

	return 0;
}