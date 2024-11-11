#include "Player.h"
#include "raylib.h"
#include "GameCamera.h"
#include "GameControls.h"
#include "Gameplay.h"

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

int Player::GetTypeId() const
{
    return ET_PLAYER;
}

void Player::SetCamera(GameCamera* cam)
{
    m_cam = cam;
}

void Player::UpdateEntity(bool doNotMove, bool doNotAnimation)
{
    if (!doNotMove)
        Move();

    Entity::UpdateEntity();
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

    float length = Vector2Length({ moveX, moveY });

    Vector3 toAdd = Vector3RotateByQuaternion({ 0.0f, 0.0f, length }, GetRot());


    pos += toAdd;
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