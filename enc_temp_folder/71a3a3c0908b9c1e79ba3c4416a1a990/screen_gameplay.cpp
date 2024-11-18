#include "rlights.h"
#include "BattyEngine.h"
#include "Constants.h"
#include "GameCamera.h"
#include "LevelMgr.h"
#include "ControllerMgr.h"
#include "EntityMgr.h"
#include "Ground.h"
#include "LevelObj.h"
#include "GameControls.h"
#include "Player.h"
#include "World.h"
#include "PowerUp.h"

static int finishScreen = 0;

static constexpr float FOV = 80.0f;
static float curFoV = FOV;
static GameCamera cam(true, curFoV);
static bool worldCam = false;
static bool justToggledCam = false;

static constexpr int TOT_LEVELS = 2;
static int curLevel = 1;

static Light lights[MAX_LIGHTS] = { 0 };

static Player* player;
static World* world;

static constexpr int MAX_ENV_ENTITIES = 32;
static int envEntitiesIdx = 0;
static Entity* environmentalEntities[MAX_ENV_ENTITIES];

static int powerUpsCollected = 0;

void MakeLevel(int level)
{
	// add environmental objects here
	LevelMgr::Instance().AddEnvObj(new Ground(1500));

    if (level == 1)
    {
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 300.0f, 0.0f, -500.0f }, 200.0f, 1000.0f, 1000.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -500.0f, 0.0f, -500.0f}, 200.0f, 1000.0f, 1000.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({-300.0f, 0.0f, 400.0f}, 500.0f, 100.0f, 100.0f));

        float radius = 10.0f;
        PowerUp* powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 200.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        _ASSERT(envEntitiesIdx < 32);
        EntityMgr::Instance().AddEntity(powerUp);
    }
}

void InitGameplayScreen()
{
	finishScreen = 0;

    memset(environmentalEntities, 0, sizeof(Entity*) * MAX_ENV_ENTITIES);

	// create one light point above
	lights[0] = CreateLight(LIGHT_POINT, { 1000.0f, 1000.0f, 1000.0f }, Vector3Zero(), { 128, 128, 128, 255 }, g_lighting);

    constexpr int NUM_MODELS = 1;
    const char* animModels[NUM_MODELS] = { Player::S_PLAYER };

    AnimationMgr::Instance().LoadMultipleModelAnimations(animModels, NUM_MODELS);

    player = new Player();
    player->SetCamera(&cam);
    EntityMgr::Instance().AddEntity(player);

    world = new World(player, true);
    EntityMgr::Instance().AddEntity(world, false);

    player->SetWorld(world);

	MakeLevel(curLevel);
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
        for (int i = 0; i < MAX_ENV_ENTITIES; i++)
        {
            if (environmentalEntities[i] != nullptr)
            {
                delete environmentalEntities[i];
                environmentalEntities[i] = nullptr;
            }
        }
        MakeLevel(curLevel);
    }
#endif // DEBUG

    for (auto& entity : EntityMgr::Instance().m_entities)
    {
        entity->UpdateEntity();
    }

    if (TOGGLE_CAM.GetPressed())
    {
        worldCam = !worldCam;
        justToggledCam = true;
    }

    if (world->m_attached || !worldCam)
    {
        cam.FollowEntity3rdPerson(*player, TICK, { 0.0f, 100.0f, -80.0f }, justToggledCam);
    }
    else
        cam.FollowEntity(*world, TICK, { 0.0f,100.0f,-80.0f});
    justToggledCam = false;
    // Update the shader with the camera view vector
    float cameraPos[3] = { cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z };
    SetShaderValue(g_lighting, g_lighting.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    bool allDone = true;
    for (int i = 0; i < envEntitiesIdx; i++)
    {
        if (environmentalEntities[i]->isSpawned())
        {
            allDone = false;
            break;
        }
    }

    if (allDone)
        printf("stuff, next level\n");
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

    DrawRectangleGradientH(50, GetRenderHeight() - 200, 500, 50, GREEN, RED);
    double power = player->GetThrowPowerUp();
    //printf("power %f\n", power);
    int startMod = (int)(power * 250.0);
    DrawRectangle(50 + startMod, GetRenderHeight() - 200, 500 - startMod, 50, BLACK);
}

void UnloadGameplayScreen()
{
    delete player;
}

int FinishGameplayScreen()
{
    return finishScreen;
}