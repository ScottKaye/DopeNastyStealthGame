#include <iostream>
#include <climits>

#include "Constants.h"
#include "Game.h"
#include "Player.h"
#include "System.h"

const float MOVEMENT_FRICTION = 0.9F;

float Distance(const Vec2& a, const Vec2& b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return std::sqrt(dx*dx + dy*dy);
}

Player::Player(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex)
	, mSpeed(20)
{
}

bool Player::Update(float dt) {
	const Uint8* keys = System::GetKeyStates();

	// Movement
	if (keys[SDL_SCANCODE_D]) {
		Velocity.x += mSpeed * dt;
	}
	if (keys[SDL_SCANCODE_A]) {
		Velocity.x -= mSpeed * dt;
	}
	if (keys[SDL_SCANCODE_W]) {
		Velocity.y -= mSpeed * dt;
	}
	if (keys[SDL_SCANCODE_S]) {
		Velocity.y += mSpeed * dt;
	}

	bool up, down, left, right;
	up = down = left = right = true;

	unsigned radius = HitRadius();

	SDL_Rect hitbox = { Center.x - radius, Center.y - radius, radius * 2, radius * 2 };

	// Check collisions against all walls
	for (Wall* w : Game::CurrentLevel->Walls) {
		SDL_Rect overlap;
		WallPlane p = w->Plane();

		if (SDL_IntersectRect(&w->GetRect(), &hitbox, &overlap)) {

			if (overlap.w * overlap.h < 50) continue;

			if (abs(overlap.w) < abs(overlap.h)) {
				if (Velocity.x < 0 && WallPlaneCollisions::CollidesLeft(p)) {
					left = false;
				}

				if (Velocity.x > 0 && WallPlaneCollisions::CollidesRight(p)) {
					right = false;
				}
			}
			else {
				if (Velocity.y < 0 && WallPlaneCollisions::CollidesUp(p)) {
					up = false;
				}

				if (Velocity.y > 0 && WallPlaneCollisions::CollidesDown(p)) {
					down = false;
				}
			}
		}
	}

	Velocity.x *= 0.9f;
	Velocity.y *= 0.9f;

	if ((Velocity.x > 0 && right) || (Velocity.x < 0 && left))
		Center.x += Velocity.x;

	if ((Velocity.y > 0 && down) || (Velocity.y < 0 && up))
		Center.y += Velocity.y;

	return false;
}

void Player::Destroy() {
	Game::DestroyPlayer();
}