#include "PowerUp.h"
#include "raylib.h"
#include "Gameplay.h"

PowerUp::PowerUp(Mesh mesh, float radius, Vector3 pos)
	: Entity(mesh, 1.0f, true, true, true, pos),
	m_radius(radius)
{
	SetTransformAndBb();
}

bool PowerUp::collisionCheck(BoundingBox bb)
{
	return CheckCollisionBoxSphere(bb, m_pos, m_radius);
}

int PowerUp::GetTypeId() const
{
	return ET_POWERUP;
}
