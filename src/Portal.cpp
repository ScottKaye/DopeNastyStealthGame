#include <iostream>

#include "Portal.h"

Portal::Portal(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex) {
	SetHitRadius(10);
}

Portal::~Portal() {

}

bool Portal::Update(float dt) {
	return true;
}

void Portal::loadNextLevel() {

}