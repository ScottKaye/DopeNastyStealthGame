#pragma once

#include "GameState.h"
#include "Button.h"
#include "Texture.h"

class EndGame : public GameState {
	Texture*            mBtnEndGameTex;
	Button*             mBtnEndGame;

public:
	EndGame(Game* game);
	~EndGame() override;

	bool                Initialize() override;

	bool                Update(float dt) override;
	void                Draw(SDL_Renderer* renderer) override;

	void		        OnKeyDown(const SDL_KeyboardEvent& kbe) override;
	void		        OnMouseDown(const SDL_MouseButtonEvent& mbe) override;
};