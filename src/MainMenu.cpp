#include "MainMenu.h"
#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

#include <iostream>

MainMenu::MainMenu(Game* game)
	: GameState(game)
	, mBtnNewTex(NULL)
	, mBtnResumeTex(NULL)
	, mBtnExitTex(NULL)
	, mBtnNew(NULL)
	, mBtnResume(NULL)
	, mBtnExit(NULL)
	, mBtnMainScreen(NULL)
{
}

MainMenu::~MainMenu() {
	Texture::Destroy(mBtnNewTex);
	Texture::Destroy(mBtnResumeTex);
	Texture::Destroy(mBtnExitTex);
	Texture::Destroy(mBtnMainScreenTex);

	delete mBtnNew;
	delete mBtnResume;
	delete mBtnExit;
	delete mBtnMainScreen;
}

bool MainMenu::Initialize() {
	SDL_Renderer* renderer = System::GetRenderer();

	mBtnNewTex = Texture::Load("media/button_new.png", renderer);
	mBtnResumeTex = Texture::Load("media/button_resume.png", renderer);
	mBtnExitTex = Texture::Load("media/button_exit.png", renderer);
	mBtnMainScreenTex = Texture::Load("media/splash.png", renderer);

	mBtnNew = new Button(mBtnNewTex);
	mBtnResume = new Button(mBtnResumeTex);
	mBtnExit = new Button(mBtnExitTex);
	mBtnMainScreen = new Button(mBtnMainScreenTex, 0, 0);

	return true;
}

void MainMenu::Update(float dt)
{
}

void MainMenu::Draw(SDL_Renderer* renderer)
{
	if (mGame->GetGameplayState()->IsActive()) {
		mGame->GetGameplayState()->Draw(renderer);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 192);
		SDL_RenderFillRect(renderer, NULL);

		mBtnResume->Draw(renderer);
	}
	else {
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderClear(renderer);
	}

	mBtnMainScreen->Draw(renderer);
}

void MainMenu::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		mGame->EnterGameplay();
		break;
	}
}

void MainMenu::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
	if (mbe.button == SDL_BUTTON_LEFT) {

		if (mGame->GetGameplayState()->IsActive()) {
			if (mBtnResume->Contains(mbe.x, mbe.y)) {
				mGame->EnterGameplay();
				return;
			}
		}

		if (mBtnNew->Contains(mbe.x, mbe.y)) {
			mGame->GetGameplayState()->LoadLevel();     // reset everything
			mGame->EnterGameplay();
			return;
		}

		if (mBtnMainScreen->Contains(mbe.x, mbe.y)) {
			mGame->GetGameplayState()->LoadLevel();
			mGame->EnterGameplay();
			return;
		}

		if (mBtnExit->Contains(mbe.x, mbe.y)) {
			System::Quit();
		}
	}
}
