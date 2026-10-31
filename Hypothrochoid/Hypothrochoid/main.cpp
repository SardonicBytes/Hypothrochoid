#include <raylib.h>
#include <raymath.h>

#include "DuckTools.h"
#include <vector>

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 1000

//Settings
float R = 400; // outer radius
float ratio = 0.375f; // inner radius as a fraction of outer radius
Vector2 drawOffset = Vector2{0,75}; // offsets of points from small circle at alpha 0

Color colors[] = { RED, GREEN, BLUE, WHITE };
int colorIndex = 1;
Color color = colors[colorIndex]; // colour of drawing

const int HIGH_PERF_MAX_DRAW_POINTS = 2000;
const float HIGH_PERF_DRAW_RATE = 0.005f;
const int LOW_PERF_MAX_DRAW_POINTS = 50;
const float LOW_PERF_DRAW_RATE = 0.15f;

int maxDrawPoints = HIGH_PERF_MAX_DRAW_POINTS;
float drawRate = HIGH_PERF_DRAW_RATE;
bool lowPerformanceMode = false;

// Amount changed per key press
const float RADIUS_INCREMENT = 10;
const float RATIO_INCREMENT = 0.025f;
const float SPEED_INCREMENT = 10;

// Limits
const float MIN_R = 50;
const float MAX_R = 480;
const float MIN_RATIO = 0.05f;
const float MAX_RATIO = 0.95f;
const float MIN_SPEED = 0;
const float MAX_SPEED = 1000;

//Runtime
float nextDrawTime = 0;
float r = R * ratio; // inner radius


float alpha = 0;
float innerRotation = 0;
Vector2 Cen = Vector2{ WINDOW_SIZE_X / 2,WINDOW_SIZE_Y / 2 };
Vector2 cen;

float deltaTime;
float moveSpeed = 100;
float innerRotateSpeed = -moveSpeed * (R - r) / r;

std::vector<Vector2> drawPoints;
Vector2 drawPoint = { r, 0 };


float Slider(Rectangle bounds, float value, float min, float max)
{
	Vector2 mouse = GetMousePosition();

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
		CheckCollisionPointRec(mouse, bounds))
	{
		float percentage = (mouse.x - bounds.x) / bounds.width;

		if (percentage < 0.0f) percentage = 0.0f;
		if (percentage > 1.0f) percentage = 1.0f;

		value = min + percentage * (max - min);
	}

	return value;
}

// Clears the drawing and restarts it from alpha 0
void ResetDrawing()
{
	drawPoints.clear();
	alpha = 0;
	innerRotation = 0;
	nextDrawTime = 0;
}

void Settings()
{
	// Snapshot so we can tell if anything changed this frame
	float oldR = R;
	float oldRatio = ratio;
	float oldMoveSpeed = moveSpeed;
	int oldColorIndex = colorIndex;
	bool oldLowPerformanceMode = lowPerformanceMode;

	// Outer radius
	if (IsKeyPressed(KEY_UP)) R += RADIUS_INCREMENT;
	if (IsKeyPressed(KEY_DOWN)) R -= RADIUS_INCREMENT;
	R = Clamp(R, MIN_R, MAX_R);

	// Inner to outer ratio
	if (IsKeyPressed(KEY_RIGHT)) ratio += RATIO_INCREMENT;
	if (IsKeyPressed(KEY_LEFT)) ratio -= RATIO_INCREMENT;
	ratio = Clamp(ratio, MIN_RATIO, MAX_RATIO);

	// Rotation speed
	if (IsKeyPressed(KEY_W)) moveSpeed += SPEED_INCREMENT;
	if (IsKeyPressed(KEY_S)) moveSpeed -= SPEED_INCREMENT;
	moveSpeed = Clamp(moveSpeed, MIN_SPEED, MAX_SPEED);

	// Colour cycle
	if (IsKeyPressed(KEY_C))
	{
		colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
		color = colors[colorIndex];
	}

	// Performance modes
	if (IsKeyPressed(KEY_L))
	{
		lowPerformanceMode = true;
		drawRate = LOW_PERF_DRAW_RATE;
		maxDrawPoints = LOW_PERF_MAX_DRAW_POINTS;
	}
	if (IsKeyPressed(KEY_H))
	{
		lowPerformanceMode = false;
		drawRate = HIGH_PERF_DRAW_RATE;
		maxDrawPoints = HIGH_PERF_MAX_DRAW_POINTS;
	}

	r = R * ratio;
	innerRotateSpeed = -moveSpeed * (R - r) / r;

	if (R != oldR || ratio != oldRatio || moveSpeed != oldMoveSpeed ||
		colorIndex != oldColorIndex || lowPerformanceMode != oldLowPerformanceMode)
	{
		ResetDrawing();
	}
}

void DrawControls()
{
	int x = 10;
	int y = 10;
	int size = 20;
	int spacing = 24;

	DrawText("UP / DOWN    - Outer radius", x, y, size, LIGHTGRAY); y += spacing;
	DrawText("LEFT / RIGHT - Inner ratio", x, y, size, LIGHTGRAY); y += spacing;
	DrawText("W / S        - Rotation speed", x, y, size, LIGHTGRAY); y += spacing;
	DrawText("C            - Cycle colour", x, y, size, LIGHTGRAY); y += spacing;
	DrawText("L            - Low performance mode", x, y, size, LIGHTGRAY); y += spacing;
	DrawText("H            - High performance mode", x, y, size, LIGHTGRAY); y += spacing * 2;

	DrawText(TextFormat("Outer radius: %.0f", R), x, y, size, color); y += spacing;
	DrawText(TextFormat("Inner ratio: %.3f (r = %.0f)", ratio, r), x, y, size, color); y += spacing;
	DrawText(TextFormat("Speed: %.0f", moveSpeed), x, y, size, color); y += spacing;
	DrawText(lowPerformanceMode ? "Mode: Low Performance" : "Mode: High Performance", x, y, size, color);
}

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
			drawPoints.erase(drawPoints.begin(), drawPoints.end() - maxDrawPoints); // Inneffecient. best to switch to queue, but may need to change DrawLineStrip to support it.
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

		Settings();
		DrawShape();
		DrawControls();

		EndDrawing();
	}

	return 0;
}
