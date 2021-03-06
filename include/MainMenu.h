#pragma once

#include "GameState.h"
#include "Button.h"
#include "Texture.h"

class MainMenu : public GameState {
	Texture*            mBtnNewTex;
	Texture*            mBtnResumeTex;
	Texture*            mBtnExitTex;
	Texture*			mBtnMainScreenTex;

	Button*             mBtnNew;
	Button*             mBtnResume;
	Button*             mBtnExit;
	Button*				mBtnMainScreen;

public:
	MainMenu(Game* game);
	~MainMenu() override;

	bool                Initialize() override;

	bool                Update(float dt) override;
	void                Draw(SDL_Renderer* renderer) override;

	void		        OnKeyDown(const SDL_KeyboardEvent& kbe) override;
	void		        OnMouseDown(const SDL_MouseButtonEvent& mbe) override;
};