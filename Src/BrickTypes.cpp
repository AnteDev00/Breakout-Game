#include "BrickTypes.h"
#include "AssetManager.h"
#include <vector>

// ------------------------------------ Solid BRICK Class -------------------------------------------

SolidBrick::SolidBrick(std::string m_Id) :
	Object(), m_HitPoints(0), m_HitSound(nullptr), m_Id(m_Id)
{
}

SolidBrick::SolidBrick(std::string _Id, std::string _TexturePath, int _HitPoints, std::string _HitSoundPath)
	: Object()
{
	// Load assets
	LoadTexture(_TexturePath); // Loads texture into Object's m_Texture
	m_HitSound = AssetManager::LoadSound(_HitSoundPath.c_str());

	// Load width and height
	AssetManager::SetSizeFromTexure(m_Texture, m_Rect);

	// Set values
	m_Id = _Id;
	m_HitPoints = _HitPoints; // (atoi - Ascii TO Integer)
}

SolidBrick::~SolidBrick()
{
	if (m_HitSound != nullptr)
	{
		Mix_FreeChunk(m_HitSound);
	}

}


void SolidBrick::OnCollision(UI& _UI)
{
	// Plays hit sound
	AssetManager::PlaySound(m_HitSound);

	if (m_HitPoints == 0)
		return;

	
}


void SolidBrick::OnRender()
{
	// If it is Empty Brick
	if (m_Texture == nullptr)
		return;

	// Renders the bricks to the screen
	AssetManager::RenderTexture(m_Texture, NULL, &m_Rect);
}

void SolidBrick::SetPosition(const int& _x, const int& _y)
{
	m_Rect.x = _x;
	m_Rect.y = _y;
}

void SolidBrick::SetWidth(const int& _Width)
{
	m_Rect.w = _Width;
}

void SolidBrick::SetHeight(const int& _Height)
{
	m_Rect.h = _Height;
}

void SolidBrick::Reset(const std::vector<SolidBrickType*>& _BrickTypes)
{
	// So that empty bricks are avoided
	if (GetId()[0] == 'I' || GetId()[0] == '_' )
	{
		return;
	}
	
	// Reset size 
	AssetManager::SetSizeFromTexure(m_Texture, m_Rect);
	
	for (SolidBrickType* type : _BrickTypes)
	{
		if (type->GetId() == m_Id)
		{
			// Reset lives
			m_HitPoints = type->GetHitPoints();
		}
	}
}

// --------------------------------- Fragile BRICK Class -------------------------------------------

FragileBrick::FragileBrick(std::string _Id, std::string _TexturePath, int _HitPoints, std::string _HitSoundPath, std::string _BreakSoundPath, int _BreakScore)
	: SolidBrick(_Id, _TexturePath, _HitPoints, _HitSoundPath)
{
	m_BreakSound = AssetManager::LoadSound(_BreakSoundPath.c_str()); // Played when ball breaks brick
	m_BreakScore = _BreakScore;
}

FragileBrick::~FragileBrick()
{
	
}


void FragileBrick::OnCollision(UI& _UI)
{
	// Plays hit sound
	AssetManager::PlaySound(this->m_HitSound);

	// Reduce hit points
	m_HitPoints--;

	// If it has no hit points
	if (m_HitPoints == 0)
	{
		// Plays break sound
		AssetManager::PlaySound(m_BreakSound);
		// Add score
		_UI.IncreaseScore(m_BreakScore);
		// "Delete it"
		m_Rect.w = 0;
		m_Rect.h = 0;
	}
}

void FragileBrick::OnRender()
{
	AssetManager::RenderTexture(m_Texture, NULL, &m_Rect);
}


// --------------------------------- Solid Brick TYPE Class -----------------------------------------------

SolidBrickType::SolidBrickType(std::string _Id, std::string _TexturePath, std::string _HitPoints, std::string _HitSoundPath)
	: m_Id(_Id), m_TexturePath(_TexturePath), m_HitPoints(_HitPoints), m_HitSoundPath(_HitSoundPath)
{

}
SolidBrickType::~SolidBrickType()
{
}


std::string SolidBrickType::GetId() const
{
	return m_Id;
}

int SolidBrickType::GetHitPoints() const
{
	return atoi(m_HitPoints.c_str());
}

std::string SolidBrickType::GetTexturePath() const
{
	return m_TexturePath;
}

std::string SolidBrickType::GetHitSoundPath() const
{
	return m_HitSoundPath;
}

// --------------------------------- Fragile Brick TYPE Class -----------------------------------------------

FragileBrickType::FragileBrickType(std::string _Id, std::string _TexturePath, std::string _HitPoints,
	std::string _HitSoundPath, std::string _BreakSoundPath, std::string _BreakScore)
	: SolidBrickType(_Id, _TexturePath, _HitPoints, _HitSoundPath), m_BreakSoundPath(_BreakSoundPath),
	m_BreakScore(atoi(_BreakScore.c_str()))
{

}

FragileBrickType::~FragileBrickType()
{
}


std::string FragileBrickType::GetBreakSoundPath() const
{
	return m_BreakSoundPath;
}

int FragileBrickType::GetBreakScore() const
{
	return m_BreakScore;
}


