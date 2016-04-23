#include <iostream>
#include <string>

#include "Constants.h"
#include "Game.h"
#include "Text.h"

// Resolve externs


// Private globals


Game::Game()
	: mGameplayState(NULL)
	, mMainMenuState(NULL)
	, mCurrentState(NULL)
{ }

bool Game::Initialize() {
	std::cout << "## Initializing game." << std::endl;

	// Set some window properties
	System::SetWindowSize(50 * 16, 50 * 12);
	System::SetWindowTitle("Dope-Nasty Stealth Game");

	// Get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	// Load all textures

	//
	// create all game states
	//
	mGameplayState = new Gameplay(this);
	if (!mGameplayState->Initialize()) {
		return false;
	}

	mMainMenuState = new MainMenu(this);
	if (!mMainMenuState->Initialize()) {
		return false;
	}

	// set initial state
	mCurrentState = mMainMenuState;

	// Create spatial hash map
	mSpatial = new Spatial(System::GetWindowWidth(), System::GetWindowHeight(), 50);



	return true;
}

void Game::Shutdown() {
	std::cout << "Shutting down game (from actually within game.cpp)" << std::endl;

	mGameplayState->Shutdown();
	mMainMenuState->Shutdown();

	delete mGameplayState;
	delete mMainMenuState;
}

void Game::Draw(SDL_Renderer* renderer) {
	// Clear the screen
	mCurrentState->Draw(renderer);

	//
	// Background
	// Effects, debug stuff
	//

	// Drawing the actual spatial boundaries makes a cool grid effect, I think I'll keep it
	
}


void Game::Update(float dt)
{
	mCurrentState->Update(dt);
}

void Game::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
	mCurrentState->OnKeyDown(kbe);
}

void Game::OnKeyUp(const SDL_KeyboardEvent& kbe)
{
	mCurrentState->OnKeyUp(kbe);
}

void Game::OnMouseDown(const SDL_MouseButtonEvent& mbe)
{
	mCurrentState->OnMouseDown(mbe);
}

void Game::OnMouseUp(const SDL_MouseButtonEvent& mbe)
{
	mCurrentState->OnMouseUp(mbe);
}

void Game::OnMouseMotion(const SDL_MouseMotionEvent& mme)
{
	mCurrentState->OnMouseMotion(mme);
}

void Game::EnterMainMenu()
{
	mCurrentState = mMainMenuState;
}

void Game::EnterGameplay()
{
	mCurrentState = mGameplayState;
}
