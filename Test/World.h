#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "Entity.h"
class Mesh;
class Player;

class World : public Entity
{
public:
	World(Mesh mesh, const Player* atlas, bool attached);

	void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false) override;

	void Move();

	void Throw();

	bool collisionCheck(BoundingBox bb) override;

	int GetTypeId() const override;

	static constexpr float DEFAULT_RADIUS = 30.0f;

protected:
	float m_radius = DEFAULT_RADIUS;
	bool m_attached = true;
	const Player* m_player = nullptr;
};

#endif // !WORLD_H_INCLUDED
