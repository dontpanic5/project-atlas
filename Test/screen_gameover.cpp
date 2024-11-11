#include "raylib.h"

static int finishScreen = 0;

void InitGameOverScreen()
{
	finishScreen = 0;
}

void UpdateGameOverScreen()
{
}

void DrawGameOverScreen()
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
	DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
}

void UnloadGameOverScreen()
{
}

int FinishGameOverScreen()
{
	return finishScreen;
}