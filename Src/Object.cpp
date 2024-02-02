#include "Object.h"
#include "AssetManager.h"

// -------------------------------- ABSTRACT BASE CLASS ---------------------------------------

Object::Object() : m_Texture(nullptr), m_Rect({ 0, 0, 0, 0 })
{
}

Object::Object(const int& _x, const int& _y, std::string _TexturePath)
{
	// Set x and y position
	m_Rect.x = _x;
	m_Rect.y = _y;
	
	// Load texture 
	LoadTexture(_TexturePath);

	// Sets width and height based on Texture
	AssetManager::SetSizeFromTexure(m_Texture, m_Rect);
}

Object::~Object()
{
    // Frees all allocated memory
	if (m_Texture != nullptr)
	{
		SDL_DestroyTexture(m_Texture);
	}
}


void Object::LoadTexture(const std::string& _TexturePath)
{
	// Securty check
	if (_TexturePath == "")  
	{
		std::cerr << "Texture path is empty!" << std::endl;
		std::exit(EXIT_FAILURE); // If it cant load texture, exit
	}
	
	// Load texture from file
	m_Texture = AssetManager::LoadTexturePNG(_TexturePath.c_str(), Breakout::m_Renderer); // Objects's texture

}


