#include "World.h"
#include "raylib.h"
#include "Constants.h"
#include "LevelMgr.h"
#include "EnvironmentalObject.h"
#include "Gameplay.h"
#include "Player.h"

World::World(Mesh mesh, const Player* atlas, bool attached)
	: Entity(mesh, 1.0f, true, true, true, { 0.0f, 0.0f, 0.0f}),
	m_attached(attached), m_player(atlas)
{
	_ASSERT(atlas);

	Texture2D world = LoadTexture("resources/Screenshot 2024-11-15 202922.png");

	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, world);

	Move();

	SetTransformAndBb();
}

void World::UpdateEntity(bool doNotMove, bool doNotAnimation)
{
	Move();

	if (GetPos().y < -100.0f)
	{
		m_attached = true;
		Move();
	}



	Entity::UpdateEntity(false, false);
}

void World::Move()
{
	if (m_attached)
	{
		m_velocity = Vector3Zero();
		m_atRest = false;
		Vector3 atlasPos = m_player->GetPos();
		atlasPos.y += 70.0f;
		SetPos(atlasPos);
	}
	else if (!m_atRest)
	{
		Vector3 worldAccel = { 0, GRAVITY_METERS, 0 };

		if (m_throwAccel != Vector3Zero())
		{
			worldAccel += m_throwAccel;
			m_throwAccel = Vector3Zero();
		}

		m_velocity += (worldAccel * TICK); // do this to bring it back to m/s^2

		CheckLevelCollisions();

		SetPos(GetPos() + m_velocity);
	}
}

void World::Throw(Vector3 direction)
{
	m_attached = false;
	m_throwAccel = direction;
}

void World::CheckLevelCollisions()
{
	for (auto& envObj : LevelMgr::Instance().m_envObjs)
	{
		if (!envObj->IsCollidable())
			continue;
		BoundingBox objBb = envObj->GetBoundingBox();
		if (collisionCheck(objBb))
		{
			Ray world;
			world.position = GetPos();
			world.direction = Vector3Normalize(m_velocity);

			RayCollision rc = envObj->GetRayCollision(world);

			m_velocity = Vector3Reflect(m_velocity, rc.normal) * 0.8f;

			if (Vector3Length(m_velocity) < 1.0f)
				m_atRest = true;
		}
	}
}

bool World::collisionCheck(BoundingBox bb)
{
	return CheckCollisionBoxSphere(bb, m_pos, m_radius);
}

int World::GetTypeId() const
{
	return ET_WORLD;
}

float World::GetRadius() const
{
	return m_radius;
}
