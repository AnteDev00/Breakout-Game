#pragma once
#include "Declarations.h"

#include "SDL.h"
#include "Level.h"

class UI 
{
private:
    int m_Lives;
    int m_Score;
    int m_CurrentLevel;
private:
    SDL_Rect m_LivesRect;
	SDL_Rect m_ScoreRect;
	SDL_Rect m_CurrentLevelRect;
private:
	SDL_Texture* m_LivesTexture;
	SDL_Texture* m_ScoreTexture;
	SDL_Texture* m_CurrentLevelTexture;
private:
    std::string m_FontName;
    SDL_Color m_FontColor;
private:
    bool m_StatusChanged; // indicated if something in the UI needs to change
public:
    UI(const Level& _Level, const std::string& _FontPath);
    ~UI();
    void OnRender();

    void PrepareFontTextures();
    
    int GetLives() const;
    void IncreaseScore(int _value);
    void ReduceLife();
    
    void ResetUI();
};