#include "World.h"
#include "raylib.h"
#include "Gameplay.h"
#include "Player.h"

World::World(Mesh mesh, const Player* atlas, bool attached)
	: Entity(mesh, 1.0f, true, true, true, { 0.0f, 0.0f, 0.0f}),
	m_attached(attached), m_player(atlas)
{
	_ASSERT(atlas);

	Move();

	SetTransformAndBb();
}

void World::UpdateEntity(bool doNotMove, bool doNotAnimation)
{
	Move();

	Entity::UpdateEntity(false, false);
}

void World::Move()
{
	if (m_attached)
	{
		Vector3 atlasPos = m_player->GetPos();
		atlasPos.y += 70.0f;
		SetPos(atlasPos);
	}
	else
	{
		SetPos(GetPos() + m_velocity);
	}
}

void World::Throw()
{
	m_attached = false;

	m_velocity = m_player->GetForward();
}

bool World::collisionCheck(BoundingBox bb)
{
	return CheckCollisionBoxSphere(bb, m_pos, m_radius);
}

int World::GetTypeId() const
{
	return ET_WORLD;
}
