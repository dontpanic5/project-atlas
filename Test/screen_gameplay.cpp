#include "rlights.h"
#include "BattyEngine.h"
#include "Constants.h"
#include "GameCamera.h"
#include "LevelMgr.h"
#include "ControllerMgr.h"
#include "EntityMgr.h"
#include "Ui.h"
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

static constexpr int MAX_ENV_ENTITIES = 128;
static int envEntitiesIdx = 0;
static Entity* environmentalEntities[MAX_ENV_ENTITIES];

static int powerUpsCollected = 0;
static bool inLevelTransition = false;

void MakeLevel(int level)
{
	// add environmental objects here
	LevelMgr::Instance().AddEnvObj(new Ground(2500));
    float radius = 10.0f;
    PowerUp* powerUp = nullptr;
    switch (level)
    {
    case 1:
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 300.0f, 0.0f, -500.0f }, 200.0f, 1000.0f, 1000.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -500.0f, 0.0f, -500.0f }, 200.0f, 1000.0f, 1000.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, 400.0f }, 500.0f, 100.0f, 100.0f));

        radius = 10.0f;
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 200.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 100.0f, radius, 200.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
#ifndef PLATFORM_WEB
        _ASSERT(envEntitiesIdx < 32);
#endif // !PLATFORM_WEB
        break;
    case 2:
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -500.0f, 0.0f, 400.0f }, 1000.0f, 600.0f, 100.0f));

        radius = 10.0f;
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, 100.0f, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, 200.0f, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -200.0f, 150.0f, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 200.0f, 150.0f, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
#ifndef PLATFORM_WEB
        _ASSERT(envEntitiesIdx < 32);
#endif // !PLATFORM_WEB
        break;
    case 3:
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, 200.0f }, 600.0f, 50.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, 300.0f }, 100.0f, 50.0f, 200.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, 500.0f }, 600.0f, 50.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 200.0f, 0.0f,  300.0f }, 100.0f, 50.0f, 200.0f));

        radius = 10.0f;
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 100.0f, radius, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -100.0f, radius, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 450.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        break;
    case 4:
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, -200.0f }, 600.0f, 50.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, -100.0f }, 100.0f, 50.0f, 200.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -300.0f, 0.0f, 100.0f }, 600.0f, 50.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 200.0f, 0.0f,  -100.0f }, 100.0f, 50.0f, 200.0f));

        radius = 10.0f;
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, -400.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -400.0f, radius, 0.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 400.0f, radius, 0.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        break;
    case 5:
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 1000.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 100.0f, radius, 1000.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -100.0f, radius, 1000.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 1100.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 0.0f, radius, 1200.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 250.0f, 0.0f, -600.0f }, 100.0f, 1000.0f, 1200.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -250.0f, 0.0f, -600.0f }, 500.0f, 1000.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -350.0f, 0.0f, -600.0f }, 100.0f, 1000.0f, 1200.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -250.0f, 0.0f, 500.0f }, 500.0f, 100.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -250.0f, 100.0f, 500.0f }, 150.0f, 500.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 150.0f, 100.0f, 500.0f }, 100.0f, 200.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -250.0f, 300.0f, 500.0f }, 700.0f, 500.0f, 100.0f));
        break;
    case 6:
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 100.0f, radius, 350.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);

        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { 285.0f, radius, 375.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);

        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -225.0f, radius, 300.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -200.0f, radius, 300.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);
        powerUp = new PowerUp(GenMeshSphere(radius, 20, 20), radius, { -225.0f, radius, 275.0f });
        environmentalEntities[envEntitiesIdx++] = powerUp;
        EntityMgr::Instance().AddEntity(powerUp);

        LevelMgr::Instance().AddEnvObj(new LevelObj({ -450.0f, 0.0f, -500.0f }, 900.0f, 1000.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -450.0f, 0.0f, -400.0f }, 100.0f, 1000.0f, 800.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -450.0f, 0.0f, 400.0f }, 900.0f, 200.0f, 100.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 350.0f, 0.0f, -400.0f }, 100.0f, 1000.0f, 800.0f));

        LevelMgr::Instance().AddEnvObj(new LevelObj({ 225.0f, 0.0f, 300.0f }, 150.0f, 100.0f, 50.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 125.0f, 0.0f, 150.0f }, 100.0f, 100.0f, 250.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ 0.0f, 0.0f, 150.0f }, 150.0f, 100.0f, 150.0f));
        LevelMgr::Instance().AddEnvObj(new LevelObj({ -100.0f, 0.0f, 75.0f }, 100.0f, 100.0f, 325.0f));

        LevelMgr::Instance().AddEnvObj(new LevelObj({ -375.0f, 0.0f, 75.0f }, 225.0f, 100.0f, 100.0f));
        break;
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

    cam.FollowEntity3rdPerson(*player, TICK, { 0.0f, 75.0f, -80.0f }, true);

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

    if (RELOAD_LEVEL.GetPressed())
    {
        inLevelTransition = false;
        LevelMgr::Instance().Unload();
        for (int i = 0; i < MAX_ENV_ENTITIES; i++)
        {
            if (environmentalEntities[i] != nullptr)
            {
                delete environmentalEntities[i];
                environmentalEntities[i] = nullptr;
                EntityMgr::Instance().m_entities.pop_back();
            }
        }
        MakeLevel(curLevel);
        world->resetRadius();
        player->SetPos(Vector3Zero());
    }

    if (inLevelTransition)
    {
        if (THROW.GetPressed())
        {
            inLevelTransition = false;
            world->resetRadius();
            curLevel++;
            LevelMgr::Instance().Unload();

            for (int i = 0; i < MAX_ENV_ENTITIES; i++)
            {
                if (environmentalEntities[i] != nullptr)
                {
                    delete environmentalEntities[i];
                    environmentalEntities[i] = nullptr;
                    EntityMgr::Instance().m_entities.pop_back();
                }
            }

            MakeLevel(curLevel);
            player->SetPos(Vector3Zero());
            cam.FollowEntity3rdPerson(*player, TICK, { 0.0f, 75.0f, -80.0f }, true);
        }
    }

    if (!inLevelTransition)
    {
        for (auto& entity : EntityMgr::Instance().m_entities)
        {
            entity->UpdateEntity();
        }
    }
    else
    {
        world->LevelTransition();
    }

    if (TOGGLE_CAM.GetPressed())
    {
        worldCam = !worldCam;
        justToggledCam = true;
    }

    if (!inLevelTransition && (world->m_attached || !worldCam))
    {
        cam.FollowEntity3rdPerson(*player, TICK, { 0.0f, 75.0f, -80.0f }, justToggledCam);
    }
    else
        cam.FollowEntity(*world, TICK, { 0.0f,100.0f,-80.0f});
    //printf("cam pos x: %f y: %f z: %f\n", cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z);
    justToggledCam = false;
    // Update the shader with the camera view vector
    float cameraPos[3] = { cam.GetPosition().x, cam.GetPosition().y, cam.GetPosition().z };
    SetShaderValue(g_lighting, g_lighting.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    bool allDone = true;
    for (int i = 0; i < envEntitiesIdx; i++)
    {
        if (environmentalEntities[i] && environmentalEntities[i]->isSpawned())
        {
            allDone = false;
            break;
        }
    }

    if (allDone)
    {
        inLevelTransition = true;
    }
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

    if (inLevelTransition)
    {
        char text[512] = "You have grown the World to a magnificent size.\n" \
            "Now you must repeat your task as is your eternal punishment.\n";
        DrawUiText(text, 0.1f, 0.25f, FontSize::m);
    }
}

void UnloadGameplayScreen()
{
    delete player;
}

int FinishGameplayScreen()
{
    return finishScreen;
}