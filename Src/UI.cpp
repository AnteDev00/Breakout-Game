#include "UI.h"
#include <iostream>
#include "AssetManager.h"

UI::UI(const Level& _Level, const std::string& _FontPath)
	: m_Lives(5), m_Score(0), m_CurrentLevel(1), m_FontName(_FontPath),
	m_ScoreTexture(nullptr), m_LivesTexture(nullptr), m_CurrentLevelTexture(nullptr),
	m_FontColor({ 150, 150, 150 }), m_ScoreRect({ 0, 0, 0, 0 }), m_LivesRect({ 0, 0, 0, 0 }), m_CurrentLevelRect({ 0, 0, 0, 0 }),
	m_StatusChanged(false)
{
	// Init Font lib
	TTF_Init();
	
	// Load the textures
	PrepareFontTextures();

	// Position UI elements
	SDL_Rect levelRect = _Level.GetBoundingBox();

	// Position Score
	m_ScoreRect = { levelRect.x -25, levelRect.y + levelRect.h + 100, 0, 0 };
	AssetManager::SetSizeFromTexure(m_ScoreTexture, m_ScoreRect);

	// Position Lives
	m_LivesRect = m_ScoreRect;
	m_LivesRect.x = levelRect.x + 200;

	// Position current Level
	m_CurrentLevelRect = m_ScoreRect;
	m_CurrentLevelRect.x = m_LivesRect.x + 200;
}

UI::~UI()
{
	TTF_Quit();
}

void UI::OnRender()
{
	// Prepare textures
	if (m_StatusChanged)
	{
		PrepareFontTextures();
		m_StatusChanged = false;
	}

	// Render them
	AssetManager::RenderTexture(m_ScoreTexture, NULL, &m_ScoreRect);
	AssetManager::RenderTexture(m_LivesTexture, NULL, &m_LivesRect);
	AssetManager::RenderTexture(m_CurrentLevelTexture, NULL, &m_CurrentLevelRect);
}

void UI::PrepareFontTextures()
{
	int fontSize = 16;
	m_LivesTexture = AssetManager::TextureFromText("LIVES: " + std::to_string(m_Lives), m_FontName, m_FontColor, fontSize);
	m_ScoreTexture = AssetManager::TextureFromText("SCORE: " + std::to_string(m_Score), m_FontName, m_FontColor, fontSize);
	m_CurrentLevelTexture = AssetManager::TextureFromText("LEVEL: " + std::to_string(m_CurrentLevel), m_FontName, m_FontColor, fontSize);

	AssetManager::SetSizeFromTexure(m_ScoreTexture, m_ScoreRect);
	AssetManager::SetSizeFromTexure(m_LivesTexture, m_LivesRect);
	AssetManager::SetSizeFromTexure(m_CurrentLevelTexture, m_CurrentLevelRect);
}


void UI::IncreaseScore(int _value)
{
	m_Score += _value;
	m_StatusChanged = true;
}

int UI::GetLives() const
{
	return m_Lives;
}

void UI::ReduceLife()
{
	m_Lives--;
	m_StatusChanged = true;
}

void UI::ResetUI()
{
	if (m_Lives <= 0)
	{
		m_Lives = 5;
		m_Score = 0;
		m_CurrentLevel = 1;
	}
}
