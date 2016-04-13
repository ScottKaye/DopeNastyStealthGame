#pragma once

#include <SDL.h>

#include "Entity.h"
#include "Vec2.h"

enum WallPlane {
	WP_All,
	WP_Left,
	WP_Right,
	WP_Top,
	WP_Bottom,
	WP_TopBottom,
	WP_LeftRight,
	WP_None
};

class WallPlaneCollisions {
public:
	static bool CollidesDown(WallPlane p) {
		return p == WallPlane::WP_All || p == WallPlane::WP_TopBottom || p == WallPlane::WP_Top;
	}
	static bool CollidesUp(WallPlane p) {
		return p == WallPlane::WP_All || p == WallPlane::WP_TopBottom || p == WallPlane::WP_Bottom;
	}
	static bool CollidesLeft(WallPlane p) {
		return p == WallPlane::WP_All || p == WallPlane::WP_LeftRight || p == WallPlane::WP_Right;
	}
	static bool CollidesRight(WallPlane p) {
		return p == WallPlane::WP_All || p == WallPlane::WP_LeftRight || p == WallPlane::WP_Left;
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