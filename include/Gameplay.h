#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "GameState.h"
#include "System.h"
#include "Player.h"


#include <vector>

class Gameplay : public GameState
{
	Texture*                mShuttleTex;
	Texture*                mShotTex;

	Player*				    mPlayer;

	

	bool                    mIsActive;

public:
	Gameplay(Game* game);
	~Gameplay() override;

	bool                    IsActive() const { return mIsActive; }

	bool                    Initialize() override;
	void                    Shutdown() override;

	void                    LoadLevel();
	void                    ClearLevel();

	void                    Update(float dt) override;
	void                    Draw(SDL_Renderer* renderer) override;

	void		            OnKeyDown(const SDL_KeyboardEvent& kbe) override;

	float                   WorldLeft() const { return 0.0f; }
	float                   WorldRight() const { return (float)System::GetWindowWidth(); }
	float                   WorldTop() const { return 0.0f; }
	float                   WorldBottom() const { return (float)System::GetWindowHeight(); }
};

#endif
