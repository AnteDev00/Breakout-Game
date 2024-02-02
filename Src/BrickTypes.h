#pragma once
#include "Declarations.h"

#include "Object.h"
#include "UI.h"

#include "SDL_mixer.h"

#include <string>
#include <vector>

// Brick instances 
// Used as brick objects on screen as you would expect
// collisions, rendering, etc.

// Impenetrable Brick type
class SolidBrick : public Object
{
public:
    SolidBrick(std::string _m_Id); // Used for creating "empty" brick
    SolidBrick(std::string _Id, std::string _TexturePath, int _HitPoints, std::string _HitSoundPath);
    virtual ~SolidBrick();
    // Methods
    void OnCollision(UI& _UI);
    void OnRender() override;
	// Resets brick to its original state
    void Reset(const std::vector<SolidBrickType*>& _BrickTypes);
public:
	// Setters
    void SetPosition(const int& _x, const int& _y);
    void SetWidth(const int& _Width);
    void SetHeight(const int& _Height);
public:
    // Getters
    inline int GetX() const { return m_Rect.x; }
    inline int GetY() const { return m_Rect.y; }
    inline int GetWidth() const { return m_Rect.w; }
    inline int GetHeigth() const { return m_Rect.h; }
    inline SDL_Rect GetRect() const { return m_Rect; }
    inline std::string GetId() const { return m_Id; }
public:
protected:
    // Data
    std::string m_Id;
    int m_HitPoints;
    Mix_Chunk* m_HitSound;
};

// Fragile Brick type  (Soft,Medium and Hard bricks)
class FragileBrick : public SolidBrick
{
public:
    FragileBrick(std::string _Id, std::string _TexturePath, int _HitPoints, std::string _HitSoundPath,
        std::string _BreakSoundPath, int _BreakScore);
    ~FragileBrick();
    // Methods
	void OnCollision(UI& _UI);
    void OnRender() override;
private:
    // Data
    Mix_Chunk* m_BreakSound;
    int m_BreakScore;
};



// Brick Type Classes
// These classes are used to store BrickTypes data from from XML
// so we can use them when creating brick instances and when we reset level


// Impenetrable Brick type
class SolidBrickType
{
public:
    SolidBrickType(std::string _Id, std::string _TexturePath, std::string _HitPoints, std::string _HitSoundPath);
    virtual ~SolidBrickType();
public:
    // Methods
    std::string GetId() const;
    int GetHitPoints() const;
    std::string GetTexturePath() const;
    std::string GetHitSoundPath() const;
protected:
    // Data
    std::string m_Id;
    std::string m_HitPoints;
    std::string m_TexturePath;
    std::string m_HitSoundPath;
};

// Fragile Brick type  (Soft,Medium and Hard bricks)
class FragileBrickType : public SolidBrickType
{
public:
    FragileBrickType(std::string _Id, std::string _TexturePath, std::string _HitPoints, std::string _SoundPath,
        std::string _BreakSound, std::string _BreakScore);
    ~FragileBrickType();
public:
    // Methods
    std::string GetBreakSoundPath() const;
    int GetBreakScore() const;
private:
    // Data
    std::string m_BreakSoundPath;
    int m_BreakScore;
};
