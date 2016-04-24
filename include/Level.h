#pragma once

#include <vector>
#include <SDL.h>

#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "Portal.h"

class Level {
	void LoadWalls(const std::string&);
	void LoadPaths(const std::string&);

public:
	Level(const std::string&);
	~Level();

	bool Map[16][12] = { 0 };
	Portal* EndPortal = NULL;
	Player* MainPlayer = NULL;
	std::vector<Entity*> Entities;
	std::vector<Wall*> Walls;

	bool Raycast(const Entity*, const Entity*) const;
	void Draw(SDL_Renderer*);
	void Update(float);
};