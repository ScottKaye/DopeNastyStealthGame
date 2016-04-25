#include <iostream>

#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

// Resolve externs
Texture*				Gameplay::PlayerTex;
Texture*				Gameplay::EnemyTex;
Texture*				Gameplay::PortalTex;
Level*					Gameplay::CurrentLevel;
bool					Gameplay::DrawHitboxes;
bool					Gameplay::GodMode;

Gameplay::Gameplay(Game* game)
	: GameState(game)
	, mLevelIndex(1)
{
	Gameplay::DrawHitboxes = false;
	Gameplay::GodMode = false;
}

Gameplay::~Gameplay() {
	std::cout << "## Shutting down game." << std::endl;

	// Delete level
	delete CurrentLevel;

	// Destroy all textures
	Texture::Destroy(PlayerTex);
	Texture::Destroy(EnemyTex);
	Texture::Destroy(PortalTex);
}

bool Gameplay::Initialize() {
	// Get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	// Load all textures
	PlayerTex = Texture::Load("media/player.png", renderer);
	EnemyTex = Texture::Load("media/enemy.png", renderer);
	PortalTex = Texture::Load("media/portal.png", renderer);

	return true;
}

void Gameplay::LoadLevel() {
	// Start level
	CurrentLevel = new Level("levels/" + std::to_string(mLevelIndex));
}

void Gameplay::NextLevel() {
	if (mLevelIndex >= mMaxLevels) {
		// You win!
		mGame->EnterEndGame();
		return;
	}
	else {
		delete CurrentLevel;
		++mLevelIndex;
		LoadLevel();
	}
}

bool Gameplay::Update(float dt) {
	// Update level
	if (CurrentLevel->Complete()) {
		NextLevel();
	}

	return CurrentLevel->Update(dt);
}

void Gameplay::Draw(SDL_Renderer* renderer) {
	// Clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Draw level
	CurrentLevel->Draw(renderer);
}

void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe) {
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		System::Quit();
		break;
	case SDLK_b:
		Gameplay::DrawHitboxes = !Gameplay::DrawHitboxes;
		break;
	case SDLK_g:
		Gameplay::GodMode = !Gameplay::GodMode;
		break;
	}
}