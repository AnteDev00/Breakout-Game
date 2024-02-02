#pragma once
#include "Declarations.h"

#include "BrickTypes.h"
#include "Ball.h"

#include <iostream>
#include <vector>

class BrickManager
{
public:
    BrickManager(const int& rowCount, const int& columnCount,
        const int& m_RowSpacing, const int& columnSpacing, const std::string& brickSequence);
    ~BrickManager();
	void OnUpdate(Ball& _Ball, UI& _UI); 	// Handles Ball-Brick Collisions 
    void OnRender();
    // Methods
    inline void AddBrickType(SolidBrickType* _BrickType) { m_BrickTypes.push_back(_BrickType); }
    inline size_t GetBrickTypeCount() const { return m_BrickTypes.size(); }
public:
    // Methods
    void CreateLevel(const SDL_Rect& _BoundingBox);
    bool FilterBricks();
    bool CreateBricks();
    void PositionBricks(const SDL_Rect& _BoundingBox);
    void ResetBricks();
private:
    void ResolveCollision(Ball& _Ball, SolidBrick* _Brick);
private:
    // Data
    int m_RowCount;
    int m_ColumnCount;
    int m_RowSpacing;
    int m_ColumnSpacing;
    std::string m_BrickSequence;
    SDL_Rect m_BricksBorder;
private:
    // BrickInstances
    std::vector<SolidBrick*> m_BrickInstances;
    std::vector<SolidBrickType*> m_BrickTypes;
};

