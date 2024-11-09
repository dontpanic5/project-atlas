#include "rlights.h"
#include "GameCamera.h"

static int finishScreen = 0;

static constexpr float FOV = 80.0f;
static float curFoV = FOV;
static GameCamera cam(true, curFoV);

static Light lights[MAX_LIGHTS] = { 0 };

void InitGameplayScreen()
{
	finishScreen = 0;


}