#pragma once
#include "Declarations.h"

#include "Breakout.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include <string>

#include "SDL_ttf.h"

// Helps us manage anything related to assets 
class AssetManager
{
public:
	static SDL_Texture* LoadTexturePNG(const std::string& filepath, SDL_Renderer* ren);

	static void RenderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest);

	static Mix_Chunk* LoadSound(const std::string& filepath);

	static void PlaySound(Mix_Chunk* sound);

	static void SetSizeFromTexure(SDL_Texture* const m_Texture, SDL_Rect& m_Rect);

	static SDL_Texture* TextureFromText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize);
};

