#include <iostream>

#include "Constants.h"
#include "Game.h"
#include "Player.h"
#include "System.h"

const float MOVEMENT_FRICTION = 0.9F;

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

	return false;
}

void Player::Destroy() {
	Game::DestroyPlayer();
}