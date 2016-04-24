#include <iostream>

#include "EndGame.h"
#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"

EndGame::EndGame(Game* game)
	: GameState(game)
	, mBtnEndGameTex(NULL)
	, mBtnEndGame(NULL)
{
}

EndGame::~EndGame() {
	Texture::Destroy(mBtnEndGameTex);

	delete mBtnEndGame;
}

bool EndGame::Initialize() {
	SDL_Renderer* renderer = System::GetRenderer();

	mBtnEndGameTex = Texture::Load("media/endsplash.png", renderer);
	mBtnEndGame = new Button(mBtnEndGameTex);

	return true;
}

bool EndGame::Update(float dt) {
	return true;
}

void EndGame::Draw(SDL_Renderer* renderer) {
	mGame->GetGameplayState()->Draw(renderer);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 192);
	SDL_RenderFillRect(renderer, NULL);

	mBtnEndGame->Draw(renderer);
}

void EndGame::OnKeyDown(const SDL_KeyboardEvent& kbe) {

}

void EndGame::OnMouseDown(const SDL_MouseButtonEvent& mbe) {
	if (mbe.button == SDL_BUTTON_LEFT) {
		if (mBtnEndGame->Contains(mbe.x, mbe.y)) {
			System::Quit();
		}
	}
}