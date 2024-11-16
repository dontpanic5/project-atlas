#ifndef POWER_UP_H_INCLUDED
#define POWER_UP_H_INCLUDED

#include "Entity.h"

class PowerUp : public Entity
{
public:
	PowerUp(Mesh mesh, float radius, Vector3 pos);

	bool collisionCheck(BoundingBox bb) override;

	int GetTypeId() const override;

protected:
	float m_radius = 10.0f;
};

#endif // !POWER_UP_H_INCLUDED
