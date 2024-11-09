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

void MakeLevel()
{
	// add environmental objects here
	//LevelMgr::Instance().AddEnvObj( );
}

int main(void)
{
	SetLogicCb(UpdateLogic);
	SetDrawCb(UpdateDrawFrame);

	Init("TEST");

	// TODO music
	// music = LoadMusicStream("");

	currentScreen = TITLE;
	InitTitleScreen();

	// create one light point above
	lights[0] = CreateLight(LIGHT_POINT, { 1000.0f, 1000.0f, 1000.0f }, Vector3Zero(), { 20, 20, 20, 255 }, g_lighting);

	// Add animations here
	//constexpr int NUM_MODELS = ;
	//const char* animModels[NUM_MODELS] = { };

	//AnimationMgr::Instance().LoadMultipleModelAnimations(animModels, NUM_MODELS);

	// Add entities here
	// EntityMgr::Instance().AddEntity( );

	MakeLevel();

	RunMainLoop();
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

		if (transAlpha < -0.01f)
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