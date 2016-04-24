#pragma once

#include "GameState.h"
#include "Button.h"
#include "Texture.h"

class GameOver : public GameState {
	Texture*            mBtnGameOverTex;

	Button*             mBtnContinue;
	Button*             mBtnQuit;
	Button*				mBtnGameOver;

public:
	GameOver(Game* game);
	~GameOver() override;

	bool                Initialize() override;

	bool                Update(float dt) override;
	void                Draw(SDL_Renderer* renderer) override;

	void		        OnKeyDown(const SDL_KeyboardEvent& kbe) override;
	void		        OnMouseDown(const SDL_MouseButtonEvent& mbe) override;
};