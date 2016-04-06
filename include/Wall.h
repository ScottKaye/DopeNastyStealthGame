#pragma once

#include <SDL.h>

#include "Entity.h"
#include "Vec2.h"

class Wall {
	SDL_Rect mRect;
public:
	Wall(SDL_Rect rect) : mRect(rect) { }

	SDL_Rect GetRect() const { return mRect; }
	void Draw(SDL_Renderer* renderer);

	int Left() const { return mRect.x; }
	int Right() const { return mRect.x + mRect.w; }
	int Top() const { return mRect.y; }
	int Bottom() const { return mRect.y + mRect.h; }
	Vec2 Center() const { return Vec2(mRect.x + mRect.w / 2, mRect.y + mRect.h / 2); }
};