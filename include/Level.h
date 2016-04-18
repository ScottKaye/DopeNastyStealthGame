#pragma once

#include <vector>
#include <SDL.h>

#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "Portal.h"

class Level {
	void LoadWalls(const std::string& filename);
	void LoadPaths(const std::string& filename);

public:
	Level() {};
	Level(const std::string& filename);
	~Level();

	std::vector<Wall*> Walls;
	std::vector<Enemy*> Enemies;
	std::vector<Portal*> Portals;

	void Draw(SDL_Renderer* renderer);
	void Update(float dt, Player* player);
};