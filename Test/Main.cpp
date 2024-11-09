#include "raylib.h"
#include "raymath.h"
#include "rlights.h"
#include "BattyEngine.h"
//#include "AnimationMgr.h"
//#include "EntityMgr.h"
//#include "LevelMgr.h"
#include "GameCamera.h"
#include "screens.h"

static GameCamera cam(true, 80.0f);

static Light lights[MAX_LIGHTS] = { 0 };

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
	music = LoadMusicStream("");

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
}