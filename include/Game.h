#pragma once

#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "System.h"
#include "Player.h"
#include "Random.h"
#include "Team.h"
#include "Level.h"
#include "Gameplay.h"
#include "MainMenu.h"

class GameState;

class Game {
	Gameplay*               mGameplayState;
	MainMenu*               mMainMenuState;
	GameState*              mCurrentState;

public:
	Game();
	~Game();

	bool				    Initialize();

	void				    Update(float dt);
	void				    Draw(SDL_Renderer* renderer);

	void				    OnKeyDown(const SDL_KeyboardEvent& kbe);
	void				    OnKeyUp(const SDL_KeyboardEvent& kbe);
	void				    OnMouseDown(const SDL_MouseButtonEvent& mbe);
	void				    OnMouseUp(const SDL_MouseButtonEvent& mbe);
	void				    OnMouseMotion(const SDL_MouseMotionEvent& mme);

	Gameplay*               GetGameplayState() const { return mGameplayState; }
	MainMenu*               GetMainMenuState() const { return mMainMenuState; }

	void                    EnterMainMenu();
	void                    EnterGameplay();

};