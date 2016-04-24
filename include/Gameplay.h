#pragma once

#include <vector>
#include <time.h>

#include "GameState.h"
#include "System.h"
#include "Player.h"
#include "Level.h"
#include "MainMenu.h"

class Gameplay : public GameState {
	unsigned mLevelIndex;
	unsigned const mMaxLevels = 2;

public:
	Gameplay(Game* game);
	~Gameplay() override;

	bool                    Initialize() override;

	void                    LoadLevel();

	bool                    Update(float dt) override;
	void                    Draw(SDL_Renderer* renderer) override;

	void		            OnKeyDown(const SDL_KeyboardEvent& kbe) override;

	float                   WorldLeft() const { return 0.0f; }
	float                   WorldRight() const { return (float)System::GetWindowWidth(); }
	float                   WorldTop() const { return 0.0f; }
	float                   WorldBottom() const { return (float)System::GetWindowHeight(); }
	void					NextLevel();

	static Texture*			PlayerTex;
	static Texture*			EnemyTex;
	static Texture*			PortalTex;
	static Level*			CurrentLevel;

	static bool DrawHitboxes;
	static bool GodMode;
};