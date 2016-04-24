#include <iostream>

#include "System.h"
#include "Enemy.h"

Enemy::Enemy(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex)
	, mSpeed(50) {
	SetHitRadius(10);
}

Enemy::~Enemy() {
	mPath.clear();
}

void Enemy::Draw(SDL_Renderer* renderer) const {
	Entity::Draw(renderer);

	float barWidth = 30;
	float filledWidth = (seenTime / mVigilance) * barWidth;

	// Background
	SDL_Rect seeingRect = { (int)(Center.x - barWidth / 2), (int)(Center.y - 15), (int)barWidth, 3 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 32);
	SDL_RenderFillRect(renderer, &seeingRect);

	// Filled part
	SDL_Rect filledRect = { (int)(Center.x - barWidth / 2), (int)(Center.y - 15), (int)filledWidth, 3 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderFillRect(renderer, &filledRect);
}

bool Enemy::Update(float dt) {
	if (!seeing && seenTime > 0) --seenTime;
	if (seeing && seenTime >= mVigilance) {
		seenTime = 0;
		seeing = false;
		return false;
	}

	if (mPath.size() == 0) return true;

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

	// If there is no target, do not move
	if (mTarget.x == 0 && mTarget.y == 0) {
		mTarget = Center;
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
	Center.x += dir.x * mSpeed * dt;
	Center.y += dir.y * mSpeed * dt;

	return true;
}

void Enemy::GoToNextLocation() {
	if (mPathIndex >= mPath.size() - 1) {
		mPathIndex = 0;
	}
	else ++mPathIndex;

	mDone = false;
}

void Enemy::See() {
	seeing = true;
	++seenTime;

	if (seenTime > mVigilance) seenTime = mVigilance;
}

void Enemy::UnSee() {
	seeing = false;
}