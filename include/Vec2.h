#pragma once

struct Vec2 {
	float x, y;

	Vec2()
		: x(0.0f), y(0.0f)
	{ }

	Vec2(float x, float y)
		: x(x), y(y)
	{ }

	static float Distance(const Vec2& from, const Vec2& to) {
		float xdiff = from.x - to.x;
		xdiff *= xdiff;

		float ydiff = from.y - to.y;
		ydiff *= ydiff;

		return std::sqrt(xdiff + ydiff);
	}
};