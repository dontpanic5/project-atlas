#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"

class Player : public Entity
{
public:
	Player(bool drawBounds = false);

	bool collisionCheck(BoundingBox bb);
	int GetTypeId() const;

	void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false);

	static constexpr int ANIM_IDLE = 0;
	static constexpr int ANIM_RUN = 1;

protected:
	void Move();
};
#endif // !PLAYER_H_INCLUDED
