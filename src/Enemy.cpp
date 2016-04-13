#include <iostream>

#include "Enemy.h"

Enemy::Enemy(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex)
	, mSpeed(1) {
	SetHitRadius(10);
}

Enemy::~Enemy() {
}

bool Enemy::Update(float dt) {
	// Current path action
	EnemyAction action = mPath[mPathIndex];

	// Resolve path action to vector
	if (mDone == false) {
		switch (action) {
		default:
		case EnemyAction::EA_Forward:
			switch (mDir) {
			default:
			case EnemyDirection::ED_Up:
				mTarget = Vec2(Center.x, Center.y - 50);
				break;
			case EnemyDirection::ED_Down:
				mTarget = Vec2(Center.x, Center.y + 50);
				break;
			case EnemyDirection::ED_Left:
				mTarget = Vec2(Center.x - 50, Center.y);
				break;
			case EnemyDirection::ED_Right:
				mTarget = Vec2(Center.x + 50, Center.y);
				break;
			}
			break;
		// Rotations are hardcoded rather than using scary math
		case EnemyAction::EA_TurnLeft:
			switch (mDir) {
			default:
			case EnemyDirection::ED_Up:
				mDir = ED_Left;
				break;
			case EnemyDirection::ED_Down:
				mDir = ED_Right;
				break;
			case EnemyDirection::ED_Left:
				mDir = ED_Down;
				break;
			case EnemyDirection::ED_Right:
				mDir = ED_Up;
				break;
			}
			break;
		case EnemyAction::EA_TurnRight:
			switch (mDir) {
			default:
			case EnemyDirection::ED_Up:
				mDir = ED_Right;
				break;
			case EnemyDirection::ED_Down:
				mDir = ED_Left;
				break;
			case EnemyDirection::ED_Left:
				mDir = ED_Up;
				break;
			case EnemyDirection::ED_Right:
				mDir = ED_Down;
				break;
			}
			break;
		}

		mDone = true;
	}

	// Get vector for which direction to move towards
	Vec2 dir = Vec2(mTarget.x - Center.x, mTarget.y - Center.y);

	// Normalize vector
	float hyp = std::sqrt(dir.x * dir.x + dir.y * dir.y);

	// If there is so little movement to make, it means we have reached the end of this step
	// Time to apply the next action
	if (hyp < 1) {
		GoToNextLocation();
	}

	// Do not divide by zero!
	if (hyp != 0) {
		dir.x /= hyp;
		dir.y /= hyp;
	}

	// Apply movement
	Center.x += dir.x * mSpeed;
	Center.y += dir.y * mSpeed;

	return true;
}

void Enemy::SetPath(std::vector<EnemyAction> path) {
	mPath = path;
}

void Enemy::GoToNextLocation() {
	std::cout << "going to next location" << std::endl;
	if (mPathIndex >= mPath.size() - 1) {
		mPathIndex = 0;
	}
	else ++mPathIndex;

	mDone = false;
}