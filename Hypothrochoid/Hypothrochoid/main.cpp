#include <raylib.h>
#include <raymath.h>

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 1000

int R = 400; // outer radius
int r = 100; // inner radius
Color color = GREEN; // colour of drawing

int main()
{
	InitWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "Chaos!");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		DrawCircleLines(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2,R,color);

		EndDrawing();
	}

	return 0;
}