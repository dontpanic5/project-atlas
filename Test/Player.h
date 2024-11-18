#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
class GameCamera;
class World;

class Player : public Entity
{
public:
	Player(bool drawBounds = false);

	bool collisionCheck(BoundingBox bb) override;
	bool collisionCheck(Vector3 pos, float radius) override;
	int GetTypeId() const override;

	double GetThrowPowerUp() const;

	void SetCamera(GameCamera* cam);

	void SetWorld(World* world);

	void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false);

	static constexpr const char* S_PLAYER = "resources/human_1.m3d";
	static constexpr int ANIM_IDLE = 2;
	static constexpr int ANIM_PUNCH = 4;
	static constexpr int ANIM_RUN = 6;
	static constexpr int ANIM_WALK = 8;

protected:
	void Move();

	void CheckLevelCollisions();

	GameCamera* m_cam;

	World* m_earth;

	double m_throwPowerUp = -1.0;
	double m_throwTime = -1.0;
	static constexpr double THROW_BUFFER = 1.0;
};
#endif // !PLAYER_H_INCLUDED
