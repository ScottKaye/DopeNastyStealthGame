#pragma once

#include <SDL.h>

#include "Entity.h"
#include "Vec2.h"

enum WallPlane {
	All,
	Left,
	Right,
	Top,
	Bottom,
	TopBottom,
	LeftRight,
	Air
};

class WallPlaneCollisions {
public:
	static bool CollidesDown(WallPlane p) {
		return p == WallPlane::All || p == WallPlane::TopBottom || p == WallPlane::Top;
	}
	static bool CollidesUp(WallPlane p) {
		return p == WallPlane::All || p == WallPlane::TopBottom || p == WallPlane::Bottom;
	}
	static bool CollidesLeft(WallPlane p) {
		return p == WallPlane::All || p == WallPlane::LeftRight || p == WallPlane::Right;
	}
	static bool CollidesRight(WallPlane p) {
		return p == WallPlane::All || p == WallPlane::LeftRight || p == WallPlane::Left;
	}
};

class Wall {
	SDL_Rect mRect;
	WallPlane mPlane;

public:
	Wall(SDL_Rect rect, WallPlane plane) : mRect(rect), mPlane(plane) { }

	SDL_Rect GetRect() const { return mRect; }
	void Draw(SDL_Renderer* renderer);

	int Left() const { return mRect.x; }
	int Right() const { return mRect.x + mRect.w; }
	int Top() const { return mRect.y; }
	int Bottom() const { return mRect.y + mRect.h; }
	WallPlane Plane() const { return mPlane; }
};