#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <SDL.h>

// forward declaration of Game class (can't include Game.h here!)
class Game;

class GameState
{
protected:
	Game*               mGame;

public:
	GameState(Game* game)
		: mGame(game)
	{ }

	virtual             ~GameState() {}

	virtual bool        Initialize() = 0;

	virtual bool        Update(float dt) = 0;
	virtual void        Draw(SDL_Renderer* renderer) = 0;

	virtual void	    OnWindowResized(int w, int h) {}
	virtual void		OnKeyDown(const SDL_KeyboardEvent& kbe) {}
	virtual void		OnKeyUp(const SDL_KeyboardEvent& kbe) {}
	virtual void		OnMouseDown(const SDL_MouseButtonEvent& mbe) {}
	virtual void		OnMouseUp(const SDL_MouseButtonEvent& mbe) {}
	virtual void		OnMouseMotion(const SDL_MouseMotionEvent& mme) {}
};

#endif
