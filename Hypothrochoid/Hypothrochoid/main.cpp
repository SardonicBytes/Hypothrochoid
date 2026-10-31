#include <raylib.h>
#include <raymath.h>
#include "DuckTools.h"
#include <vector>

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 1000

//Settings
int R = 400; // outer radius
int r = 150; // inner radius
Color color = GREEN; // colour of drawing
Vector2 drawOffset = Vector2{0,75}; // offsets of points from small circle at alpha 0
int maxDrawPoints = 1000;


//Runtime
float nextDrawTime = 0;
float drawRate = 0.05f;

float alpha = 0;
float innerRotation = 0;
Vector2 Cen = Vector2{ WINDOW_SIZE_X / 2,WINDOW_SIZE_Y / 2 };
Vector2 cen;

float deltaTime;
float moveSpeed = 100;
float innerRotateSpeed = -moveSpeed * (R - r) / r;

std::vector<Vector2> drawPoints;
Vector2 drawPoint = { (float)r, 0 };

void DrawShape()
{
	deltaTime = GetFrameTime();
	cen = Cen + Vector2{ PolarToVector(R - r,alpha) };

	//DrawCircleLines(Cen.x,Cen.y,R,color);
	//DrawCircleLines(cen.x,cen.y,r,color);

	alpha += deltaTime * moveSpeed;
	innerRotation += deltaTime * innerRotateSpeed;


	drawPoint = cen + Vector2Rotate(drawOffset, innerRotation * DEG2RAD);

	if (GetTime() > nextDrawTime)
	{
		drawPoints.push_back(drawPoint);
		if (drawPoints.size() > maxDrawPoints)
		{
			drawPoints.erase(drawPoints.begin()); // Inneffecient. best to switch to queue, but may need to change DrawLineStrip to support it.
		}
		nextDrawTime = GetTime() + drawRate;
	}

	DrawLineStrip(drawPoints.data(), drawPoints.size(), color);
}

int main()
{
	InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Chaos!");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawShape();

		EndDrawing();
	}

	return 0;
}