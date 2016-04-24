#include <iostream>

#include "Portal.h"
#include "Gameplay.h"

Portal::Portal(const Vec2& pos, const Texture* tex)
	: Entity(pos, tex) {
	SetHitRadius(10);
}

Portal::~Portal() {

}

bool Portal::Update(float dt) {
	// Im going to bed
	return true;
}

void Portal::loadNextLevel() {

}