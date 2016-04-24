#include <iostream>

#include "MainMenu.h"
#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

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
	Texture::Destroy(mBtnMainScreenTex);
	delete mBtnMainScreen;
}

bool MainMenu::Initialize() {
	SDL_Renderer* renderer = System::GetRenderer();

	mBtnMainScreenTex = Texture::Load("media/splash.png", renderer);
	mBtnMainScreen = new Button(mBtnMainScreenTex, 0, 0);

	return true;
}

bool MainMenu::Update(float dt) {
	return true;
}

void MainMenu::Draw(SDL_Renderer* renderer) {
	mBtnMainScreen->Draw(renderer);
}

void MainMenu::OnKeyDown(const SDL_KeyboardEvent& kbe) {
	switch (kbe.keysym.sym) {
	case SDLK_ESCAPE:
		mGame->EnterGameplay();
		break;
	}
}

void MainMenu::OnMouseDown(const SDL_MouseButtonEvent& mbe) {
	if (mbe.button == SDL_BUTTON_LEFT) {
		if (mBtnMainScreen->Contains(mbe.x, mbe.y)) {
			mGame->EnterGameplay();
			mGame->GetGameplayState()->LoadLevel();
			return;
		}

		if (mBtnExit->Contains(mbe.x, mbe.y)) {
			System::Quit();
		}
	}
}