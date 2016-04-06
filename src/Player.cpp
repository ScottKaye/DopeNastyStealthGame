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
	: Entity(pos, tex, Team::Player)
	, mSpeed(50)
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

	Velocity.x *= 0.9f;
	Velocity.y *= 0.9f;

	Center.x += Velocity.x;
	Center.y += Velocity.y;

	unsigned radius = HitRadius();

	SDL_Rect hitbox = { Center.x - radius, Center.y - radius, radius * 2, radius * 2 };

	// Check collisions against all walls
	for (Wall* w : Game::CurrentLevel->Walls) {
		SDL_Rect overlap;
		if (SDL_IntersectRect(&w->GetRect(), &hitbox, &overlap)) {
			if (abs(overlap.w) < abs(overlap.h)) {
				// Intersection left or right

				if (Velocity.x < 0) {
					// Moving left
					//Center.x = overlap.x + overlap.w + radius;
					Center.x += overlap.w;
				}
				else {
					// Moving right
					//Center.x = overlap.x - radius;
					Center.x -= overlap.w;
				}
			}
			else {
				// Top or bottom

				if (Velocity.y < 0) {
					// Moving up
					//Center.y = overlap.y + overlap.h + radius;
					Center.y += overlap.h;
				}
				else {
					// Moving down
					//Center.y = overlap.y - radius;
					Center.y -= overlap.h;
				}
			}
		}
	}

	return false;
}

void Player::Destroy() {
	Game::DestroyPlayer();
}