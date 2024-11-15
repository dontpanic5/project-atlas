#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
class GameCamera;
class World;

class Player : public Entity
{
public:
	Player(bool drawBounds = false);

	bool collisionCheck(BoundingBox bb);
	int GetTypeId() const;

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

	GameCamera* m_cam;

	World* m_earth;
};
#endif // !PLAYER_H_INCLUDED
