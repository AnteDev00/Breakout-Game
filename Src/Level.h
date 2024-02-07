#pragma once
#include "Declarations.h"

#include "BrickManager.h"
#include "BrickTypes.h"
#include "Ball.h"
#include "UI.h"

#include <vector>
#include <string>

class Level 
{
public:
    Level(const int& _ScreenWidth, const int& _ScreenHeight,const std::string& _XMLFilepath);
    ~Level();
    void OnUpdate(Ball& _Ball, UI& _UI);
    void OnRender();
    // Methods
    bool LoadLevel(const std::string& _LevelFilepath);
    void ResetLevel() const;
public:
	// Getters
	inline SDL_Rect GetBoundingBox() const { return m_BoundingBox; }
	inline Mix_Chunk* GetHitSound() const { return m_HitSound; }
private:
    // Methods
    void SetLevelRect(const int& _ScreenWidth, const int& _ScreenHeight);
	void SetLevelBoundingBox();
private:
    // LevelData
	SDL_Rect m_Rect; // Level Rect
	SDL_Rect m_BoundingBox; // Collision detection
	SDL_Texture* m_BoardTexture; // Level Texture
    Mix_Chunk* m_HitSound; // Wall hit sound
private:
    // Level Filepath
	std::string m_BoardTexturePath; // Level Texture Path
private:
    // Object data
    BrickManager* m_BrickManager;
};

