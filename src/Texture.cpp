// Texture class created by Lukasz Wawrzyniak

#include <iostream>
#include <SDL_image.h>

#include "Texture.h"

Texture::Texture(SDL_Texture* tex, int width, int height)
	: mSDLTex(tex)
	, mWidth(width)
	, mHeight(height)
{ }

Texture::~Texture() {
	if (mSDLTex) {
		SDL_DestroyTexture(mSDLTex);
	}
}

Texture* Texture::Load(const std::string& path, SDL_Renderer* renderer) {
	// Load an image into an SDL_Surface (stored in RAM)
	SDL_Surface* surf = IMG_Load(path.c_str());
	if (!surf) {
		std::cerr << "*** Failed to load image: " << IMG_GetError() << std::endl;
		return NULL;
	}

	// Create an SDL_Texture from the surface (stored in GPU DRAM)
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	// Free the surface, which is no longer needed
	SDL_FreeSurface(surf);

	if (tex) {
		// Query texture size
		int width, height;
		SDL_QueryTexture(tex, NULL, NULL, &width, &height);

		// Return a dynamically allocated Texture object
		return new Texture(tex, width, height);
	}

	// Texture creation failed
	std::cerr << "*** Failed to create texture: " << SDL_GetError() << std::endl;
	return NULL;
}

void Texture::Destroy(Texture* tex) {
	delete tex;
}