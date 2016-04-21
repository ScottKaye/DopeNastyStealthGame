#pragma once

#include <vector>

#include "Entity.h"
#include "Texture.h"
#include "Vec2.h"
#include "Random.h"
#include "Util.h"


class Portal : public Entity {


public:
	Portal(const Vec2& pos, const Texture* tex);
	~Portal();

	bool		Update(float dt) override;

	void		loadNextLevel();
};