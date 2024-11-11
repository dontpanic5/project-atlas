#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "BattyEngine.h"
#include "screens.h"

GameScreen currentScreen;

// Required variables to manage screen transitions (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

static void UpdateLogic(void); // game logic tick
static void UpdateDrawFrame(); // draw one frame

int main(void)
{
	SetLogicCb(UpdateLogic);
	SetDrawCb(UpdateDrawFrame);

	Init("TEST");

	// TODO music
	// music = LoadMusicStream("");

	currentScreen = TITLE;
	InitTitleScreen();
	InitGameplayScreen();

	RunMainLoop();

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload current screen data before closing
	switch (currentScreen)
	{
	case TITLE: UnloadTitleScreen(); break;
	case GAMEOVER: UnloadGameOverScreen(); break;
	default: break;
	}
	UnloadGameplayScreen();

	//UnloadMusicStream(music);

	DeInit();
}

// Request transition to next screen
static void TransitionToScreen(int screen)
{
	onTransition = true;
	transFadeOut = false;
	transFromScreen = currentScreen;
	transToScreen = screen;
	transAlpha = 0.0f;
}

// Update transition effect (fade-in, fade-out)
static void UpdateTransition(void)
{
	if (!transFadeOut)
	{
		transAlpha += 0.05f;

		// NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
		// For that reason we compare against 1.01f, to avoid last frame loading stop
		if (transAlpha > 1.01f)
		{
			transAlpha = 1.0f;

			// Unload current screen
			switch (transFromScreen)
			{
			case TITLE: UnloadTitleScreen(); break;
			case GAMEOVER: UnloadGameOverScreen(); break;
			default: break;
			}

			// Load next screen
			switch (transToScreen)
			{
			case TITLE: InitTitleScreen(); break;
			case GAMEOVER: InitGameOverScreen(); break;
			default: break;
			}

			currentScreen = GameScreen(transToScreen);

			// Activate fade out effect to next loaded screen
			transFadeOut = true;
		}
	}
	else  // Transition fade out logic
	{
		transAlpha -= 0.02f;

		// TODO put this back
		if (true)
		{
			transAlpha = 0.0f;
			transFadeOut = false;
			onTransition = false;
			transFromScreen = -1;
			transToScreen = -1;
		}
	}
}

// Draw transition effect (full-screen rectangle)
static void DrawTransition(void)
{
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

static void UpdateLogic()
{
	//UpdateMusicStream(music);

	if (!onTransition)
	{
		switch (currentScreen)
		{
		case TITLE:
		{
			UpdateTitleScreen();

			if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

			break;
		}
		case GAMEPLAY:
			UpdateGameplayScreen();
			break;
		}
	}
	else UpdateTransition();    // Update transition (fade-in, fade-out)
}

// draw game frame
static void UpdateDrawFrame()
{
	ClearBackground(RAYWHITE);

	switch (currentScreen)
	{
	case TITLE: DrawTitleScreen(); break;
	case GAMEPLAY: DrawGameplayScreen(); break;
	case GAMEOVER: DrawGameOverScreen(); break;
	}

	// Draw full screen rectangle in front of everything
	if (onTransition) DrawTransition();
}