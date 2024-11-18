#include "World.h"
#include "raylib.h"
#include "Constants.h"
#include "EntityMgr.h"
#include "LevelMgr.h"
#include "EnvironmentalObject.h"
#include "BattyEngine.h"
#include "Gameplay.h"
#include "Player.h"

World::World(const Player* atlas, bool attached)
	: Entity(S_MODEL, 30.0f, true, true, true),
	m_attached(attached), m_player(atlas)
{
	_ASSERT(atlas);

	Texture2D world = LoadTexture("resources/2k_earth_daymap.png");

	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, world);
	SetMaterialTexture(&m_model.materials[1], MATERIAL_MAP_DIFFUSE, world);
	m_model.materials[0].shader = g_lighting;
	m_model.materials[1].shader = g_lighting;

	Move();

	SetTransformAndBb();
	Vector3 dist = m_bb.max - m_bb.min;
	m_radius = Vector3Length(dist) / 2.0f;
}

void World::UpdateEntity(bool doNotMove, bool doNotAnimation)
{
	Move();

	if (GetPos().y < -100.0f)
	{
		m_attached = true;
		Move();
	}

	CheckEntityCollisions();

	Entity::UpdateEntity(false, false);

	Vector3 dist = m_bb.max - m_bb.min;
	m_radius = Vector3Length(dist) / 4.0f * 1.2f;
}

void World::DrawEntity(float offsetY)
{
	Entity::DrawEntity(m_radius * -1.0f);

	DrawSphereWires(m_pos, m_radius, 10, 10, GREEN);
}

void World::Move()
{
	if (m_attached)
	{
		m_velocity = Vector3Zero();
		m_atRest = false;
		Vector3 atlasPos = m_player->GetPos();
		atlasPos.y += 40.0f + m_radius;
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

			float overlap = envObj->getOverlapDistance(m_pos, m_radius);
			Vector3 toMove = rc.normal * overlap;
			SetPos(GetPos() + toMove);

			if (Vector3Length(m_velocity) < 1.5f &&
					Vector3Equals(rc.normal, { 0.0f, 1.0f, 0.0f }))
				m_atRest = true;
		}
	}
}

void World::CheckEntityCollisions()
{
	for (auto& entity : EntityMgr::Instance().m_entities)
	{
		if (!entity->isSpawned())
			continue;

		if (entity->collisionCheck(m_pos, m_radius))
		{
			if (entity->GetTypeId() == ET_POWERUP)
			{
				entity->ResetEntity();
				m_scale *= 1.5f;
			}
		}
	}
}

bool World::collisionCheck(BoundingBox bb)
{
	return CheckCollisionBoxSphere(bb, m_pos, m_radius);
}

bool World::collisionCheck(Vector3 pos, float radius)
{
	return CheckCollisionSpheres(m_pos, m_radius, pos, radius);
}

int World::GetTypeId() const
{
	return ET_WORLD;
}

float World::GetRadius() const
{
	return m_radius;
}
