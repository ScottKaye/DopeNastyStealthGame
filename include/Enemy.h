#pragma once

#include <vector>

#include "Entity.h"
#include "Texture.h"
#include "Vec2.h"
#include "EnemyAction.h"

class Enemy : public Entity {
	EnemyDirection mDir;
	std::vector<EnemyAction> mPath;
	Vec2 mTarget = Vec2(0, 0);
	bool mDone = false;
	unsigned mPathIndex = 0;
	float mSpeed;

	bool mSeeing; // If the enemy is currently looking at the player
	float mSeenTime; // How many ticks an enemy has seen the player for
	float mStupidity; // How long it takes for an enemy to catch on to the player
	float mVisionRange; // Range at which an enemy can see the player

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

	float		VisionRange() const { return mVisionRange; }
};