#include "Wall.h"

void Wall::Draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &mRect);
}