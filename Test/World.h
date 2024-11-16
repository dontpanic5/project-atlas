#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "Entity.h"
struct Mesh;
struct Vector3;
class Player;

class World : public Entity
{
public:
	World(Mesh mesh, const Player* atlas, bool attached);

	void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false) override;

	void Move();

	void Throw(Vector3 direction);

	void CheckLevelCollisions();

	bool collisionCheck(BoundingBox bb) override;

	int GetTypeId() const override;

	float GetRadius() const;

	bool m_attached = true;

	static constexpr float DEFAULT_RADIUS = 20.0f;

protected:
	float m_radius = DEFAULT_RADIUS;
	const Player* m_player = nullptr;
	Vector3 m_throwAccel = Vector3Zero();
	bool m_atRest = false;
};

#endif // !WORLD_H_INCLUDED
