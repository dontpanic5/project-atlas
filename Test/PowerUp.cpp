#include "PowerUp.h"
#include "raylib.h"
#include "Gameplay.h"
#include "BattyEngine.h"

PowerUp::PowerUp(Mesh mesh, float radius, Vector3 pos)
	: Entity(mesh, 1.0f, true, true, true, pos)
{
	Image green = GenImageColor(1024, 1024, GREEN);
	Texture tex = LoadTextureFromImage(green);
	SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, tex);
	SetMaterialShaders(g_lighting);

	SetTransformAndBb();
	Vector3 dist = m_bb.max - m_bb.min;
	m_radius = Vector3Length(dist) / 2.0f;
}

bool PowerUp::collisionCheck(BoundingBox bb)
{
	return CheckCollisionBoxSphere(bb, m_pos, m_radius);
}

bool PowerUp::collisionCheck(Vector3 pos, float radius)
{
	return CheckCollisionSpheres(m_pos, m_radius, pos, radius);
}

void PowerUp::UpdateEntity(bool doNotMove, bool doNotAnimation)
{
	Entity::UpdateEntity();

	Vector3 dist = m_bb.max - m_bb.min;
	m_radius = Vector3Length(dist) / 4.0f * 1.2f;
}

void PowerUp::DrawEntity(float offsetY)
{
	Entity::DrawEntity();

#ifdef DEBUG
	DrawSphereWires(m_pos, m_radius, 10, 10, GREEN);
#endif
}

int PowerUp::GetTypeId() const
{
	return ET_POWERUP;
}
