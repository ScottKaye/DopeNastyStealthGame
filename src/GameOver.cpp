#include "GameOver.h"
#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

#include <iostream>

GameOver::GameOver(Game* game)
	: GameState(game)
	, mBtnGameOverTex(NULL)
	, mBtnContinue(NULL)
	, mBtnQuit(NULL)
	, mBtnGameOver(NULL)
{
}

GameOver::~GameOver() {
	Texture::Destroy(mBtnGameOverTex);

	delete mBtnContinue;
	delete mBtnQuit;
	delete mBtnGameOver;
}

bool GameOver::Initialize() {
	SDL_Renderer* renderer = System::GetRenderer();

	mBtnGameOverTex = Texture::Load("media/game_over.png", renderer);

	mBtnContinue = new Button(100, 411, 342, 62);
	mBtnQuit = new Button(507, 411, 170, 62);
	mBtnGameOver = new Button(mBtnGameOverTex, 0, 0);


	return true;
}

bool GameOver::Update(float dt)
{
	return true;
}

void GameOver::Draw(SDL_Renderer* renderer)
{

	mGame->GetGameplayState()->Draw(renderer);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 192);
	SDL_RenderFillRect(renderer, NULL);


	mBtnGameOver->Draw(renderer);
}

void GameOver::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		mGame->EnterGameplay();
		break;
	}
}

void GameOver::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
	if (mbe.button == SDL_BUTTON_LEFT) {
		if (mBtnContinue->Contains(mbe.x, mbe.y)) {
			mGame->EnterGameplay();
			mGame->GetGameplayState()->LoadLevel();
			return;
		}

		if (mBtnQuit->Contains(mbe.x, mbe.y)) {
			System::Quit();
		}
	}
}
