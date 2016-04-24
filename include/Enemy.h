#pragma once

#include <vector>

#include "Entity.h"
#include "Texture.h"
#include "Vec2.h"
#include "Random.h"
#include "Util.h"
#include "EnemyAction.h"

class Enemy : public Entity {
	EnemyDirection mDir;
	std::vector<EnemyAction> mPath;
	Vec2 mTarget = Vec2(0, 0);
	bool mDone = false;
	unsigned mPathIndex = 0;
	float mSpeed;

	bool seeing = false;
	float seenTime = 0;
	float mVigilance = 300;

public:
	Enemy(const Vec2& pos, const Texture* tex);
	~Enemy() override;

	void		Draw(SDL_Renderer*) const;
	bool		Update(float dt) override;
	void		SetDirection(EnemyDirection dir) { mDir = dir; }
	void		SetPath(std::vector<EnemyAction> path) { mPath = path; }
	void		GoToNextLocation();
	void		See();
	void		UnSee();  // midnight coding
};