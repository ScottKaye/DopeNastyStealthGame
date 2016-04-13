#pragma once

#include <vector>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "System.h"
#include "Player.h"
#include "Random.h"
#include "Team.h"
#include "Spatial.h"
#include "Level.h"
#include "Gameplay.h"
#include "MainMenu.h"

class Game {
	Spatial*				mSpatial;
	Gameplay*               mGameplayState;
	MainMenu*               mMainMenuState;
	GameState*              mCurrentState;

public:
	Game();

	bool				    Initialize();
	void				    Shutdown();

	void				    Update(float dt);
	void				    Draw(SDL_Renderer* renderer);

	void				    OnKeyDown(const SDL_KeyboardEvent& kbe);
	void				    OnKeyUp(const SDL_KeyboardEvent& kbe);
	void				    OnMouseDown(const SDL_MouseButtonEvent& mbe);
	void				    OnMouseUp(const SDL_MouseButtonEvent& mbe);
	void				    OnMouseMotion(const SDL_MouseMotionEvent& mme);

	float                   WorldLeft() const { return 0.0f; }
	float                   WorldRight() const { return (float)System::GetWindowWidth(); }
	float                   WorldTop() const { return 0.0f; }
	float                   WorldBottom() const { return (float)System::GetWindowHeight(); }

	Gameplay*               GetGameplayState() const { return mGameplayState; }
	MainMenu*               GetMainMenuState() const { return mMainMenuState; }

	void                    EnterMainMenu();
	void                    EnterGameplay();

	// Public globals
	static void				DestroyEntityById(int id);
	static void				DestroyPlayer();

	static Texture*			PlayerTex;
	static Texture*			EnemyTex;

	static Player*				    MainPlayer;
	static std::vector<Entity*>		Entities;
	static Level*					CurrentLevel;
};