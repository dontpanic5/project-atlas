#ifndef LEVEL_OBJ_H_INCLUDED
#define LEVEL_OBJ_H_INCLUDED

#include "EnvironmentalObject.h"

class LevelObj : public EnvironmentalObject
{
public:
	LevelObj(Vector3 cornerPos, float width, float height, float length);

	void drawObj() override;

	bool collisionCheck(BoundingBox bb) const override;
	BoundingBox GetBoundingBox() const override;
	float getOverlapDistance(BoundingBox bb, Vector3 direction) const override;
	float getOverlapDistance(Vector3 center, float radius) const override;
	RayCollision GetRayCollision(Ray ray, bool addBuffer = false) const override;

protected:
	BoundingBox m_bb;

	static bool s_texLoaded;
	static Texture s_tex;
};

#endif // !LEVEL_OBJ_H_INCLUDED
