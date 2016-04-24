#include <iostream>
#include <string>

#include "Game.h"

Game::Game()
	: mGameplayState(NULL)
{ }

bool Game::Initialize() {
	std::cout << "## Initializing game." << std::endl;

	// Set some window properties
	System::SetWindowSize(50 * 16, 50 * 12);
	System::SetWindowTitle("Dope-Nasty Stealth Game");

	// Get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	mMainMenuState = new MainMenu(this);
	if (!mMainMenuState->Initialize()) {
		return false;
	}

	mEndGameState = new EndGame(this);
	if (!mEndGameState->Initialize()) {
		return false;
	}

	mGameOverState = new GameOver(this);
	if (!mGameOverState->Initialize()) {
		return false;
	}

	// Set initial state
	mCurrentState = mMainMenuState;

	return true;
}

Game::~Game() {
	std::cout << "Shutting down game (from actually within game.cpp)" << std::endl;

	delete mMainMenuState;
	delete mGameplayState;
	delete mEndGameState;
	delete mGameOverState;
}

void Game::Draw(SDL_Renderer* renderer) {
	mCurrentState->Draw(renderer);
}

void Game::Update(float dt) {
	bool safe = mCurrentState->Update(dt);
	if (!safe) mCurrentState = mGameOverState;
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe) {
	mCurrentState->OnKeyDown(kbe);
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe) {
	mCurrentState->OnKeyUp(kbe);
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe) {
	mCurrentState->OnMouseDown(mbe);
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe) {
	mCurrentState->OnMouseUp(mbe);
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme) {
	mCurrentState->OnMouseMotion(mme);
}

void Game::EnterMainMenu() {
	mCurrentState = mMainMenuState;
}

void Game::EnterGameplay() {
	if (mGameplayState != NULL) {
		delete mGameplayState;
		mGameplayState = NULL;
	}

	mGameplayState = new Gameplay(this);
	mGameplayState->Initialize();
	mCurrentState = mGameplayState;
}