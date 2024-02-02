#pragma once
#include "Declarations.h"

#include "SDL.h"
#include "SDL_mixer.h"

#include "Object.h"
#include "Level.h"

#include <string>


class Ball : public Object
{
public:
    Ball(const SDL_Rect& _LevelBoundingBox, std::string _TexturePath);
    ~Ball();
    // Methods
    void OnUpdate(Level& _Level, UI& UI);
    void OnRender() override;
public:
    // Setters
    inline void SetDx(const int& _Dx) { m_Dx = _Dx; }
    inline void SetDy(const int& _Dy) { m_Dy = _Dy; }
    // Getters
    inline int GetDx() const { return m_Dx; }
    inline int GetDy() const { return m_Dy; };
    inline SDL_Rect GetRect() const { return m_Rect; }
    inline Mix_Chunk* GetSound() const { return m_HitSound; }
public:
    void ResetPosition();
private:
    // Data
    int m_Dx; // X axis velocity
    int m_Dy; // Y axis velocity
private:
    Mix_Chunk* m_HitSound;
    SDL_Rect m_LevelBoundingBox; // Used for collision detection

};
