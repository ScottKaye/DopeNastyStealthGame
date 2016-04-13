#pragma once

#include <vector>
#include <math.h>

#include "Team.h"
#include "Vec2.h"
#include "Texture.h"

class Entity {
	const Texture*			mTex;
	unsigned				mId;
	static unsigned			mNextId;
	unsigned				mHitRadius;

public:
	Entity(const Vec2& pos, const Texture* tex);

	unsigned				GetId() const { return mId; }

	Vec2					Center;
	Vec2					Velocity;
	bool					Garbage = false;

	unsigned				HitRadius() const { return mHitRadius; }
	float                   Left() const { return Center.x - 0.5f * Width(); }
	float                   Right() const { return Center.x + 0.5f * Width(); }
	float                   Top() const { return Center.y - 0.5f * Height(); }
	float                   Bottom() const { return Center.y + 0.5f * Height(); }

	void					SetHitRadius(unsigned r) { mHitRadius = r; }
	void                    SetLeft(float x) { Center.x = x + 0.5f * Width(); }
	void                    SetRight(float x) { Center.x = x - 0.5f * Width(); }
	void                    SetTop(float y) { Center.y = y + 0.5f * Height(); }
	void                    SetBottom(float y) { Center.y = y - 0.5f * Height(); }

	float					Radius() const { return fmaxf(Width(), Height()) / 2; }
	float                   Width() const { return (float)mTex->GetWidth(); }
	float                   Height() const { return (float)mTex->GetHeight(); }

	// Implementation-specific methods
	virtual bool			Update(float dt);
	virtual void			Destroy();
	virtual void			Draw(SDL_Renderer* renderer) const;
	virtual bool			CollidesWith(Entity* ent) const;
};