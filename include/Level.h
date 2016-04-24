#pragma once

#include <vector>
#include <SDL.h>

#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "Portal.h"
#include "Spatial.h"

class Level {
	Spatial*				mSpatial;

	void LoadWalls(const std::string& filename);
	void LoadPaths(const std::string& filename);

public:
	Level(const std::string& filename);
	~Level();

	Portal* EndPortal = NULL;
	Player* MainPlayer = NULL;
	std::vector<Entity*> Entities;
	std::vector<Wall*> Walls;

	void Draw(SDL_Renderer* renderer);
	void Update(float dt);
};