#include <iostream>

#include "Game.h"
#include "Entity.h"

// Resolve externs
unsigned Entity::mNextId;

Entity::Entity(const Vec2& pos, const Texture* tex, Team team)
	: mTex(tex)
	, Center(pos)
	, mTeam(team)
	, mId(mNextId++)
	, mHitRadius(20)
{
}

// Return true if the entity was destroyed this frame
bool Entity::Update(float dt) {
	// "Dead" entities are in limbo

	Center.x += Velocity.x * dt;
	Center.y += Velocity.y * dt;

	return false;
}

void Entity::Draw(SDL_Renderer* renderer) const {
	// Check if we have a valid texture
	if (mTex) {
		// Compute rectangle on screen
		SDL_Rect screenRect;
		screenRect.w = mTex->GetWidth();
		screenRect.h = mTex->GetHeight();
		screenRect.x = (int)(Center.x - mTex->GetWidth() / 2);
		screenRect.y = (int)(Center.y - mTex->GetHeight() / 2);

		double angle = (atan2(Velocity.y, Velocity.x) * 180 / M_PI);

		// Draw textured rectangle
		SDL_RenderCopyEx(renderer, mTex->GetSDLTexture(), NULL, &screenRect, angle, NULL, SDL_FLIP_NONE);
	}
	else {
		// Draw a placeholder
		SDL_Rect screenRect;
		screenRect.w = 64;
		screenRect.h = 64;
		screenRect.x = (int)(Center.x - screenRect.w / 2);
		screenRect.y = (int)(Center.y - screenRect.h / 2);

		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &screenRect);
	}
}

bool Entity::CollidesWith(Entity* ent) const {
	SDL_Rect r1 = { (int)Left(), (int)Top(), (int)Width(), (int)Height() };
	SDL_Rect r2 = { (int)ent->Left(), (int)ent->Top(), (int)ent->Width(), (int)ent->Height() };

	// This is to quiet the compiler about forcibly converting from SDL_bool to bool
	return SDL_HasIntersection(&r1, &r2) ? true : false;
}

// Remove the entity from the game
void Entity::Destroy() {
	Game::DestroyEntityById(GetId());
}