#ifndef POWER_UP_H_INCLUDED
#define POWER_UP_H_INCLUDED

#include "Entity.h"

class PowerUp : public Entity
{
public:
	PowerUp(Mesh mesh, float radius, Vector3 pos);

	bool collisionCheck(BoundingBox bb) override;
	bool collisionCheck(Vector3 pos, float radius) override;

	void UpdateEntity(bool doNotMove, bool doNotAnimation) override;
	void DrawEntity(float offsetY = 0.0f) override;

	int GetTypeId() const override;

protected:
	float m_radius = -1.0f;
};

#endif // !POWER_UP_H_INCLUDED
