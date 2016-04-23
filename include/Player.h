#pragma once

#include "Entity.h"
#include "Texture.h"
#include "Vec2.h"
#include "Random.h"
#include "Util.h"

class Player : public Entity {
	float                   mSpeed;

public:
	Player(const Vec2& pos, const Texture* tex);
	float                   Speed() const { return mSpeed; }
	void                    SetSpeed(float speed) { mSpeed = speed; }

	bool					Update(float dt) override;
};