#pragma once

#include <string>
#include "SDL.h"



// ----------------------- ABSTRACT BASE CLASS ------------------------

class Object 
{
protected:
	Object(); // Used as a null constructor
    Object(const int& _x, const int& _y, std::string _TexturePath); // Calls SetSizeFromTexure(), so you dont have to call it manually
    virtual ~Object(); 	                                            // but when you use default constructor, you have to manually call it

    // Methods
	virtual void OnRender() = 0; 
    void LoadTexture(const std::string& _TexturePath);
protected:                                             
    // Data
    SDL_Rect m_Rect; 
	SDL_Texture* m_Texture; 
};

