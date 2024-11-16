#include "rlights.h"
#include "BattyEngine.h"
#include "Constants.h"
#include "GameCamera.h"
#include "LevelMgr.h"
#include "ControllerMgr.h"
#include "EntityMgr.h"
#include "Ground.h"
#include "GameControls.h"
#include "Player.h"
#include "World.h"
#include "PowerUp.h"

static int finishScreen = 0;

static constexpr float FOV = 80.0f;
static float curFoV = FOV;
static GameCamera cam(true, curFoV);

static Light lights[MAX_LIGHTS] = { 0 };

static Player* player;

static constexpr int MAX_ENV_ENTITIES = 32;
static int envEntitiesIdx = 0;
static Entity* environmentalEntities[MAX_ENV_ENTITIES];

void MakeLevel()
{
	// add environmental objects here
	LevelMgr::Instance().AddEnvObj(new Ground(1500));

    float radius = 10.0f;
    PowerUp* powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius / 2.0f, 200.0f });
    envEntitiesIdx++;
    _ASSERT(envEntitiesIdx < 32);
    environmentalEntities[envEntitiesIdx] = powerUp;
    EntityMgr::Instance().AddEntity(powerUp);
}

void InitGameplayScreen()
{
	finishScreen = 0;

    memset(environmentalEntities, 0, sizeof(Entity*) * MAX_ENV_ENTITIES);

	// create one light point above
	lights[0] = CreateLight(LIGHT_POINT, { 1000.0f, 1000.0f, 1000.0f }, Vector3Zero(), { 20, 20, 20, 255 }, g_lighting);

    constexpr int NUM_MODELS = 1;
    const char* animModels[NUM_MODELS] = { Player::S_PLAYER };

    AnimationMgr::Instance().LoadMultipleModelAnimations(animModels, NUM_MODELS);

    player = new Player();
    player->SetCamera(&cam);
    EntityMgr::Instance().AddEntity(player);

    World* world = new World(GenMeshSphere(World::DEFAULT_RADIUS, 20, 20), player, true);
    EntityMgr::Instance().AddEntity(world);

    player->SetWorld(world);

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
        for (int i = 0; i < MAX_ENV_ENTITIES; i++)
        {
            if (environmentalEntities[i] != nullptr)
            {
                delete environmentalEntities[i];
                environmentalEntities[i] = nullptr;
            }
        }
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