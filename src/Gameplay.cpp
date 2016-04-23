#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"
#include "Constants.h"

#include <iostream>

//resolve externs
Texture*				Gameplay::PlayerTex;
Texture*				Gameplay::EnemyTex;
std::vector<Entity*>	Gameplay::Entities;
Player*					Gameplay::MainPlayer;
Level*					Gameplay::CurrentLevel;
Texture*				Gameplay::PortalTex;
Portal*					Gameplay::endPortal;

Gameplay::Gameplay(Game* game)
	: GameState(game)
	, mPlayer(NULL)
	, mIsActive(false)
{
}

Gameplay::~Gameplay()
{
}

//private shit
bool showHitboxes = true;

bool Gameplay::Initialize()
{
	// get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	//
	// load all textures
	//
	PlayerTex = Texture::Load("media/player.png", renderer);
	EnemyTex = Texture::Load("media/enemy.png", renderer);
	PortalTex = Texture::Load("media/portal.png", renderer);

	
	LoadLevel();

	return true;
}

void Gameplay::Shutdown()
{
	ClearLevel();


}

void Gameplay::LoadLevel()
{
	//ClearLevel();

	//
	// Spawn player
	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.5f * System::GetWindowHeight();
	MainPlayer = new Player(spawnPos, PlayerTex);

	Entities.push_back(MainPlayer);

	//Place portal
	Vec2 portalPos;
	portalPos.x = System::GetWindowWidth() - 24;
	portalPos.y = 26;
	endPortal = new Portal(portalPos, PortalTex);
	Entities.push_back(endPortal);

	// Start level
	CurrentLevel = new Level("levels/2");

	//add the enemies
	for (Enemy* e : CurrentLevel->Enemies) {
		Entities.push_back(e);
	}

}

void Gameplay::ClearLevel()
{
	delete mPlayer;
	mPlayer = NULL;

	std::cout << "## Shutting down game." << std::endl;

	// Delete player
	delete MainPlayer;

	// Delete level
	delete CurrentLevel;

	// Delete spatial hash map
	delete mSpatial;

	delete endPortal;

	// Release audio

	// Destroy all textures
	Texture::Destroy(PlayerTex);
	Texture::Destroy(EnemyTex);
	Texture::Destroy(PortalTex);
}

void Gameplay::Update(float dt)
{
	// Update level
	CurrentLevel->Update(dt, MainPlayer);

	// Reset spatial hashmap
	mSpatial->ClearCells();
	mSpatial->Register(MainPlayer);

	// Process entities
	for (unsigned i = 0; i < Entities.size(); ++i) {
		Entity* e = Entities[i];

		// "Garbage collector", clean up and move on
		if (e->Garbage) {
			delete Entities[i];
			Entities[i] = Entities.back();
			Entities.pop_back();

			continue;
		}

		// If entity has been killed by update, move on
		if (e->Update(dt)) continue;

		mSpatial->Register(e);

		// Collisions
		std::vector<Entity*> nearby = mSpatial->GetNearby(e);
		for (unsigned j = 0; j < nearby.size(); ++j) {
			Entity* e2 = nearby[j];

			// Are we colliding with anything?
			if (e->CollidesWith(e2)) {
				// TODO player loses

				break;
			}
		}
	}
}

void Gameplay::Draw(SDL_Renderer* renderer)
{
	// clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int size = mSpatial->Cellsize();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 25);
	for (int col = 0; col < mSpatial->Cols(); ++col) {
		for (int row = 0; row < mSpatial->Rows(); ++row) {
			SDL_Rect rect = { col * size, row * size, size, size };
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	// Light up squares with entities
	// Squares with more entities will be brighter
	for (Entity* ent : Entities) {
		int r = 255;
		int g = 255;
		int b = 255;
		int op = 25;

		if (dynamic_cast<Player*>(ent) != NULL) {
			r = 0;
			g = 246;
			b = 255;
			op = 35;
		}

		int row = (int)std::floor(ent->Center.y / size);
		int col = (int)std::floor(ent->Center.x / size);

		// Light up surrounding squares a little bit as well
		for (int y = row - 1; y <= row + 1; ++y) {
			for (int x = col - 1; x <= col + 1; ++x) {
				SDL_Rect rect = { x * size, y * size, size, size };
				SDL_SetRenderDrawColor(renderer, r, g, b,
					x == col && y == row
					? op
					: (int)(op / 2));
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	//
	// Entities
	//

	// Draw entities
	for (unsigned i = 0; i < Entities.size(); ++i) {
		Entities[i]->Draw(renderer);
	}

	// Draw level
	CurrentLevel->Draw(renderer);

	//
	// Top level
	// UI
	//

	// Hitboxes
	if (showHitboxes) {

		// Raw boxes
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (unsigned i = 0; i < Entities.size(); ++i) {
			SDL_Rect rect = { (int)Entities[i]->Left(), (int)Entities[i]->Top(), (int)Entities[i]->Height(), (int)Entities[i]->Width() };
			SDL_RenderDrawRect(renderer, &rect);
		}

		// Hit radiuses
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		int density = 25;
		for (Entity* ent : Entities) {
			for (int p = 0; p < density; p++) {
				float x = ent->Center.x + ent->HitRadius() * std::cos(2 * (float)M_PI * p / density);
				float y = ent->Center.y + ent->HitRadius() * std::sin(2 * (float)M_PI * p / density);

				SDL_RenderDrawPoint(renderer, (int)x, (int)y);
			}
		}
	}


}
void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe) {
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		System::Quit();
		break;
	case SDLK_b:
		showHitboxes = !showHitboxes;
		break;
	}
}

void Gameplay::DestroyPlayer() {
	// TODO
}

void Gameplay::DestroyEntityById(int id) {
	for (unsigned i = 0; i < Entities.size(); ++i) {
		if (Entities[i]->GetId() == id) {
			Entities[i]->Garbage = true;
			break;
		}
	}
}
