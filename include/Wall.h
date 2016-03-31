#pragma once

#include <SDL.h>

#include "Entity.h"
#include "Vec2.h"

class Wall {
	SDL_Rect mRect;
public:
	Wall(SDL_Rect rect) : mRect(rect) { }

	void Draw(SDL_Renderer* renderer);
};