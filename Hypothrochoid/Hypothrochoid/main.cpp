#include <raylib.h>
#include <raymath.h>
#include "DuckTools.h"

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 1000

int R = 400; // outer radius
int r = 100; // inner radius
Color color = GREEN; // colour of drawing

int main()
{
	InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Chaos!");

	float alpha = 0;
	Vector2 Cen = Vector2{ WINDOW_SIZE_X / 2,WINDOW_SIZE_Y / 2 };
	Vector2 cen;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);


		cen = Cen + Vector2{PolarToVector(R-r,alpha)};

		DrawCircleLines(Cen.x,Cen.y,R,color);
		DrawCircleLines(cen.x,cen.y,r,color);

		EndDrawing();
	}

	return 0;
}