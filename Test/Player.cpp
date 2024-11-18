#include "Player.h"
#include "raylib.h"
#include "LevelMgr.h"
#include "EnvironmentalObject.h"
#include "GameCamera.h"
#include "GameControls.h"
#include "Gameplay.h"
#include "World.h"

Player::Player(bool drawBounds)
    : Entity(S_PLAYER, 40.0f, drawBounds, true)
{
    SetCurAnim(ANIM_IDLE);
    m_animLoop = true;
}

bool Player::collisionCheck(BoundingBox bb)
{
    // TODO why isn't this defined in Entity?
    return CheckCollisionBoxes(GetBoundingBox(), bb);
}

bool Player::collisionCheck(Vector3 pos, float radius)
{
    return CheckCollisionBoxSphere(GetBoundingBox(), pos, radius);
}

int Player::GetTypeId() const
{
    return ET_PLAYER;
}

double Player::GetThrowPowerUp() const
{
    if (m_throwPowerUp == -1.0)
        return 0.0;

    double power = GetTime() - m_throwPowerUp;
    if (power > 2)
        power = 2;
    return power;
}

void Player::SetCamera(GameCamera* cam)
{
    m_cam = cam;
}

void Player::SetWorld(World* world)
{
    m_earth = world;
}

void Player::UpdateEntity(bool doNotMove, bool doNotAnimation)
{
    if (!doNotMove)
        Move();

    Entity::UpdateEntity();

    CheckLevelCollisions();

    if (ControllerMgr::SPACEBAR.GetPressed() && m_earth->m_attached)
    {
        m_throwPowerUp = GetTime();
        //printf("m_throwPowerUp %f\n", m_throwPowerUp);
    }

    if (ControllerMgr::SPACEBAR.GetReleased() && m_earth->m_attached && m_throwPowerUp != -1.0)
    {
        Vector3 dir = { 0.0f, 1.0f, 0.8f };
        dir = Vector3RotateByQuaternion(dir, m_rot);
        dir = Vector3Normalize(dir);
        double nowTime = GetTime();
        //printf("nowTime %f\n", nowTime);
        double powerUpMult = GetThrowPowerUp();
        float power = 325.0f * (float) powerUpMult;
        dir *= power;
        m_earth->Throw(dir);
        m_throwTime = GetTime();
        m_throwPowerUp = -1.0;
    }

    if (!m_earth->m_attached && CheckCollisionBoxSphere(GetBoundingBox(), m_earth->GetPos(), m_earth->GetRadius()))
    {
        double now = GetTime();
        if (now - m_throwTime > THROW_BUFFER)
            m_earth->m_attached = true;
    }
    else if (!m_earth->m_attached && ControllerMgr::SPACEBAR.GetDown())
    {
        m_earth->m_attached = true;
    }
}

void Player::Move()
{
    float moveX = MOVE_X.GetPressed();
    float moveY = MOVE_Y.GetPressed();

    if (moveX == 0.0f && moveY == 0.0f)
        return;

    // easier to work with these like this
    moveX *= -1.0f;
    moveY *= -1.0f;
    Vector3 move = { moveX, 0.0f, moveY };
    move = Vector3Normalize(move);

    Vector3 camForward = m_cam->GetForward();
    Vector2 camForward2 = { camForward.x, camForward.z };
    Vector2 move2 = { move.x * -1.0f, move.z };

    float rotAngle = Vector2Angle(camForward2, move2);

    SetAllRot(QuaternionFromAxisAngle({ 0.0f, 1.0f, 0.0f }, rotAngle));


    Vector3 pos = GetPos();

    float length = Vector2Length({ moveX, moveY }) * 2.0f;

    Vector3 toAdd = Vector3RotateByQuaternion({ 0.0f, 0.0f, length }, GetRot());


    pos += toAdd;
    if (Vector3Length(toAdd) > 0.0f)
    SetPos(pos);

    if (DidMove() && length > 0.8f)
    {
        SetCurAnim(ANIM_RUN);
    }
    else if (DidMove())
    {
        SetCurAnim(ANIM_WALK);
    }
    else
    {
        SetCurAnim(ANIM_IDLE);
    }
}

void Player::CheckLevelCollisions()
{
    for (auto& envObj : LevelMgr::Instance().m_envObjs)
    {
        if (!envObj->IsCollidable())
            continue;
        BoundingBox bb = GetBoundingBox();
        if (envObj->collisionCheck(GetBoundingBox()))
        {
            /*
            printf("m_crashStart: %f\n", m_crashStart);
            printf("time elapsed: %f\n", GetTime() - m_crashStart);
            printf("envObj: %p, m_lastCrashObj: %p\n", envObj, m_lastCrashObj);
            printf("dist: %f\n", Vector3Distance(m_pos, m_crashPos));
            */

            Vector3 veloNorm = Vector3Normalize(GetPos() - m_prevPos);

            BoundingBox envBb = envObj->GetBoundingBox();
            Vector3 middle = Vector3Lerp(envBb.min, envBb.max, 0.5f);
            Vector3 dir = middle - GetPos();

            Ray ray;
            ray.position = GetPos();
            ray.position.y += 5.0f; // raise off the ground
            ray.direction = Vector3Normalize(dir);

            RayCollision rayCollison = GetRayCollisionBox(ray, envObj->GetBoundingBox());

            if (rayCollison.hit)
            {
                float distance = envObj->getOverlapDistance(bb, rayCollison.normal);
                Vector3 toMove = Vector3Scale(rayCollison.normal, distance);
                SetPos(Vector3Add(GetPos(), toMove));
            }

            //SetPos(m_prevPos);
        }
    }
}