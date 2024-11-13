#include "rlights.h"
#include "BattyEngine.h"
#include "Constants.h"
#include "GameCamera.h"
#include "LevelMgr.h"
#include "ControllerMgr.h"
#include "EntityMgr.h"
#include "Ground.h"
#include "Player.h"

static int finishScreen = 0;

static constexpr float FOV = 80.0f;
static float curFoV = FOV;
static GameCamera cam(true, curFoV);

static Light lights[MAX_LIGHTS] = { 0 };

static Player* player;

void MakeLevel()
{
	// add environmental objects here
	LevelMgr::Instance().AddEnvObj(new Ground(1500));
}

void InitGameplayScreen()
{
	finishScreen = 0;

	// create one light point above
	lights[0] = CreateLight(LIGHT_POINT, { 1000.0f, 1000.0f, 1000.0f }, Vector3Zero(), { 20, 20, 20, 255 }, g_lighting);

    /*constexpr int NUM_MODELS = 1;
    const char* animModels[NUM_MODELS] = { Player::S_PLAYER };

    AnimationMgr::Instance().LoadMultipleModelAnimations(animModels, NUM_MODELS);*/

    player = new Player();
    player->SetCamera(&cam);
    player->SetBillboardAnim("resources/Elementals_Leaf_ranger_Free_v1.0/animations/PNG/idle/idle_%d.png", Player::ANIM_IDLE, 12);
    player->SetBillboardAnim("resources/Elementals_Leaf_ranger_Free_v1.0/animations/PNG/run/run_%d.png", Player::ANIM_RUN, 10);
    EntityMgr::Instance().AddEntity(player);

	MakeLevel();
}

void UpdateGameplayScreen()
{
    if (ControllerMgr::ESCAPE.GetPressed())
    {
        ShowCursor();
        ControllerMgr::Instance().TrapCursor(false);
    }
    if (ControllerMgr::Instance().GetCurControlDevice() != CONTROL_DEVICE::MOUSE)
    {
        if (!Vector2Equals(GetMouseDelta(), Vector2Zero()))
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }
    if (ControllerMgr::LMB.GetPressed())
    {
        HideCursor();
        ControllerMgr::Instance().TrapCursor(true);
    }

#ifdef DEBUG
    if (RELOAD_LEVEL.GetPressed())
    {
        LevelMgr::Instance().Unload();
        MakeLevel();
    }
#endif // DEBUG

    for (auto& entity : EntityMgr::Instance().m_entities)
    {
        entity->UpdateEntity();
    }

    cam.FollowEntity3rdPerson(*player, TICK, { 0.0f, 100.0f, -80.0f });
    // Update the shader with the camera view vector
    float cameraPos[3] = { cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z };
    SetShaderValue(g_lighting, g_lighting.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
}

void DrawGameplayScreen()
{
    ClearBackground({ 40,40,40,255 });
    
    cam.Begin3DDrawing();

    for (auto& entity : EntityMgr::Instance().m_entities)
    {
        entity->DrawEntity();
    }

    for (auto& envObj : LevelMgr::Instance().m_envObjs)
    {
        envObj->drawObj();
    }

    cam.EndDrawing();
}

void UnloadGameplayScreen()
{
    delete player;
}

int FinishGameplayScreen()
{
    return finishScreen;
}