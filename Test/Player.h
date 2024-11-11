#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"

class Player : public Entity
{
public:
	Player(bool drawBounds = false);

	bool collisionCheck(BoundingBox bb);
	int GetTypeId() const;

	static constexpr const char* S_PLAYER = "resources/human_1.m3d";
	static constexpr int ANIM_IDLE = 2;
	static constexpr int ANIM_PUNCH = 4;
	static constexpr int ANIM_RUN = 6;
	static constexpr int ANIM_WALK = 8;
};
#endif // !PLAYER_H_INCLUDED
