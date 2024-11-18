#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "Entity.h"
struct Vector3;
class Player;

class World : public Entity
{
public:
	World(const Player* atlas, bool attached);

	void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false) override;
	void DrawEntity(float offsetY = 0.0f) override;

	void Move();

	void Throw(Vector3 direction);

	void CheckLevelCollisions();
	void CheckEntityCollisions();

	bool collisionCheck(BoundingBox bb) override;
	bool collisionCheck(Vector3 pos, float radius) override;

	int GetTypeId() const override;

	float GetRadius() const;

	bool m_attached = true;

protected:
	float m_radius = -1.0f;
	const Player* m_player = nullptr;
	Vector3 m_throwAccel = Vector3Zero();
	bool m_atRest = false;

	static constexpr const char* S_MODEL = "resources/earth ball.m3d";
};

#endif // !WORLD_H_INCLUDED
