#pragma once

#include "SDL.h"

#include "Object.h"

#include "Ball.h"

#include <string>

class Player : public Object
{
public:
    Player(const SDL_Rect& _LevelBoundingBox, std::string _TexturePath);
    ~Player();
    // Methods
    void OnUpdate(Ball& ball);
    void OnRender() override;

	void SetDx(const int& _Dx) { m_Dx = _Dx; }
    inline int GetDx() const { return m_Dx; }

    bool CanAccelerateLeft() const;
    bool CanAccelerateRight() const;

    void ResloveCollision(Ball& _Ball);
private:
    // Data
    int m_Dx; // velocity on X axis
    SDL_Rect m_LevelBoundingBox;
};
