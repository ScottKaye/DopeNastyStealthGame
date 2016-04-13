#include <iostream>
#include <string>

#include "Constants.h"
#include "Game.h"
#include "Text.h"

// Resolve externs
Texture*				Game::PlayerTex;
Texture*				Game::EnemyTex;
std::vector<Entity*>	Game::Entities;
Player*					Game::MainPlayer;
Level*					Game::CurrentLevel;

// Private globals
bool showHitboxes = true;

Game::Game()
{ }

bool Game::Initialize() {
	std::cout << "## Initializing game." << std::endl;

	// Set some window properties
	System::SetWindowSize(50 * 16, 50 * 12);
	System::SetWindowTitle("Dope-Nasty Stealth Game");

	// Get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	// Load all textures
	PlayerTex = Texture::Load("media/player.png", renderer);
	EnemyTex = Texture::Load("media/enemy.png", renderer);

	// Create spatial hash map
	mSpatial = new Spatial(System::GetWindowWidth(), System::GetWindowHeight(), 50);

	// Spawn player
	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.5f * System::GetWindowHeight();
	MainPlayer = new Player(spawnPos, PlayerTex);

	Entities.push_back(MainPlayer);

	// Start level
	CurrentLevel = new Level("levels/1");

	for (Enemy* e : CurrentLevel->Enemies) {
		Entities.push_back(e);
	}

	return true;
}

void Game::Shutdown() {
	std::cout << "## Shutting down game." << std::endl;

	// Delete entities
	for (unsigned i = 0; i < Entities.size(); ++i) {
		delete Entities[i];
	}
	Entities.clear();

	// Delete level
	delete CurrentLevel;

	// Delete spatial hash map
	delete mSpatial;

	// Release audio

	// Destroy all textures
	Texture::Destroy(PlayerTex);
	Texture::Destroy(EnemyTex);
}

void Game::Draw(SDL_Renderer* renderer) {
	// Clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//
	// Background
	// Effects, debug stuff
	//

	// Drawing the actual spatial boundaries makes a cool grid effect, I think I'll keep it
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

void Game::Update(float dt) {
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

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe) {
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		System::Quit();
		break;
	case SDLK_r:
		Reset();
		break;
	case SDLK_b:
		showHitboxes = !showHitboxes;
		break;
	}
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe) {
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe) {
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe) {
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme) {
}

void Game::DestroyEntityById(int id) {
	for (unsigned i = 0; i < Entities.size(); ++i) {
		if (Entities[i]->GetId() == id) {
			Entities[i]->Garbage = true;
			break;
		}
	}
}

void Game::DestroyPlayer() {
	// TODO
}

void Game::Reset() {
	for (unsigned i = 0; i < Entities.size(); ++i) delete Entities[i];
	Entities.clear();

	Initialize();
}

void Game::EnterMainMenu()
{
	mCurrentState = mMainMenuState;
}

void Game::EnterGameplay()
{
	mCurrentState = mGameplayState;
}