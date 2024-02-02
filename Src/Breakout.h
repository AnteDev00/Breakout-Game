#pragma once
#include "Declarations.h"
#include <iostream>

#include "SDL_image.h"
#include "SDL.h"

#include "UI.h"
#include "Level.h"
#include "Object.h"
#include "Player.h"

class Breakout
{
public:
	Breakout();
	~Breakout();
	void Run();
public: 
	static SDL_Window* m_Window;
    static SDL_Renderer* m_Renderer;
private:
	void OnInput();	
	void OnUpdate();	
	void OnRender();
	void ResetLevel();
private:
	Level* m_Level;
	Player* m_Player;
	Ball* m_Ball;
	UI* m_UI;  
private:
	bool m_Running; 
	// Screen size (5:4) ratio
	int m_ScreenWidth;
	int m_ScreenHeight;
private:
	//Mix_Music* m_Music;
private:
	SDL_Event m_Event;
};

