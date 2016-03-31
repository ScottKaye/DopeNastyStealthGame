#pragma once

#include <vector>
#include <SDL.h>

#include "Player.h"
#include "Wall.h"

class Level {
public:
	Level() {};
	Level(const std::string& filename);
	~Level();

	std::vector<Wall*> Walls;

	void Draw(SDL_Renderer* renderer);
	void Update(float dt, Player* player);
};