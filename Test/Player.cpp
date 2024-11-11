#include "Player.h"
#include "raylib.h"
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
