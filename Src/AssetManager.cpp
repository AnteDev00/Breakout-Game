#include "AssetManager.h"


SDL_Texture* AssetManager::LoadTexturePNG(const std::string& filepath, SDL_Renderer* ren)
{
	// In case XML file has "wrong" path, this fixes it
	std::string filepath_prefix = "";
	if (filepath.substr(0, 7) != "Assets/")
	{
		filepath_prefix = "Assets/";
	}

	// Load surface from file
	SDL_Surface* surface = IMG_Load((filepath_prefix + filepath).c_str());
	if (surface == nullptr)
	{
		std::cerr << "Unable to load surface from " << filepath << "! SDL_image Error: " << IMG_GetError() << std::endl;
		std::exit(EXIT_FAILURE); // If it cant load surface, exit
	}
	// Create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	if (texture == nullptr)
	{
		std::cerr << "Unable to create texture from surface! SDL_error: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		std::exit(EXIT_FAILURE); // If it cant load texture, exit
	}
	// Clean up surface
	SDL_FreeSurface(surface);

	// If loading was succesful, return texture
	return texture;
}


void AssetManager::RenderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dest)
{
	// Used for rendering empty bricks
	if (texture == nullptr) 
	{
		std::cerr << "Texture is nullptr" << std::endl;
		std::exit(EXIT_FAILURE);
		return;
	}
	// Render normally
	if (SDL_RenderCopy(Breakout::m_Renderer, texture, src, dest) != NULL)
	{
		std::cerr << "Failed to render texture" << SDL_GetError() << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
}

Mix_Chunk* AssetManager::LoadSound(const std::string& filepath)
{
	std::string filepath_prefix = "";
	if (filepath.substr(0, 7) != "Assets/")
	{
		filepath_prefix = "Assets/";
	}
	
	Mix_Chunk* sound = Mix_LoadWAV((filepath_prefix + filepath).c_str());
	if (sound == nullptr)
	{
		std::cerr << "Failed to load sound: " << filepath << std::endl;
		std::exit(EXIT_FAILURE);
	}
	else
	{
		return sound;
	}
	
}

void AssetManager::PlaySound(Mix_Chunk* sound)
{
	if (sound == nullptr)
	{
		std::cerr << "No sound to be played" << std::endl;
		return;
	}
	if(Mix_PlayChannel(-1, sound, 0) == -1)
	{
		std::cerr << "Failed to play sound: " << Mix_GetError() << std::endl;
		return;
	}
}

// Set width and height, from the Texture
void AssetManager::SetSizeFromTexure(SDL_Texture* const m_Texture, SDL_Rect& m_Rect)
{
	if (m_Texture == nullptr)
	{
		std::cerr << "Can't set size. The texture is nullptr!" << std::endl;
		std::exit(EXIT_FAILURE); // If it cant load texture, exit
	}
	else
		SDL_QueryTexture(m_Texture, NULL, NULL, &m_Rect.w, &m_Rect.h);
}

SDL_Texture* AssetManager::TextureFromText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize)
{
	TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);

	SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Breakout::m_Renderer, surf);

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}
