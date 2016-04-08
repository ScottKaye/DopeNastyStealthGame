#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

#include <iostream>

Gameplay::Gameplay(Game* game)
	: GameState(game)
	, mPlayer(NULL)
	, mIsActive(false)
{
}

Gameplay::~Gameplay()
{
}

bool Gameplay::Initialize()
{
	// get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	//
	// load all textures
	//

	LoadLevel();

	return true;
}

void Gameplay::Shutdown()
{
	ClearLevel();

	// destroy all textures
	Texture::Destroy(mShuttleTex);
	Texture::Destroy(mShotTex);
}

void Gameplay::LoadLevel()
{
	ClearLevel();

	//
	// spawn player
	//

	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.75f * System::GetWindowHeight();

	mPlayer = new Player(spawnPos, mShuttleTex);

	mPlayer->SetSpeed(150.0f);
}

void Gameplay::ClearLevel()
{
	delete mPlayer;
	mPlayer = NULL;

//	for (auto it = mMissiles.begin(); it != mMissiles.end(); it++) {
//		Missile* m = *it;
//		delete m;
//	}
//	mMissiles.clear();
}

void Gameplay::Update(float dt)
{
	// get world bounds
	float worldLeft = WorldLeft();
	float worldRight = WorldRight();
	float worldTop = WorldTop();
	float worldBottom = WorldBottom();

	// update player
	mPlayer->Update(dt);

	// keep the player within world bounds
	if (mPlayer->Left() < worldLeft) {
		mPlayer->SetLeft(worldLeft);
	}
	else if (mPlayer->Right() > worldRight) {
		mPlayer->SetRight(worldRight);
	}
	if (mPlayer->Top() < worldTop) {
		mPlayer->SetTop(worldTop);
	}
	else if (mPlayer->Bottom() > worldBottom) {
		mPlayer->SetBottom(worldBottom);
	}

	// update missiles


	mIsActive = true;
}

void Gameplay::Draw(SDL_Renderer* renderer)
{
	// clear the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// draw missiles

	// draw player
	mPlayer->Draw(renderer);
}

void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		std::cout << "User pressed Escape" << std::endl;
		mGame->EnterMainMenu();
		break;

	case SDLK_SPACE:
		// fire a missile
	{
		
	}
	break;
	}
}
