#include "Ball.h"
#include "AssetManager.h"

#include "SDL_mixer.h"

#include <random>


Ball::Ball(const SDL_Rect& _LevelBoundingBox, std::string _TexturePath)
	: Object(), m_Dx(-3), m_Dy(-5), m_HitSound(nullptr), m_LevelBoundingBox(_LevelBoundingBox)
{
	// Load assets
	LoadTexture(_TexturePath);
	m_HitSound = AssetManager::LoadSound("Assets/Sounds/BallSound.wav");

	// Set position
	AssetManager::SetSizeFromTexure(m_Texture, m_Rect); // w and h from texture
	m_Rect.x = _LevelBoundingBox.x + (_LevelBoundingBox.w / 2) - (m_Rect.w / 2);
	m_Rect.y = _LevelBoundingBox.y + (_LevelBoundingBox.h / 2) - (m_Rect.h / 2);

}

Ball::~Ball()
{
	if (m_HitSound != nullptr)
	{
		Mix_FreeChunk(m_HitSound);
	}
}


void Ball::OnUpdate(Level& _Level, UI& _UI)
{

	if (m_Dx == 0 && m_Dy == 0)
	{
		ResetPosition();
		return;
	}

	// -----------------------------------------------------------------------
	// ----------------- Handles BALL<->LEVEL COLLISIONS ---------------------
	// -----------------------------------------------------------------------

	bool hasHit = false;
	// If Ball Exits through the bottom "wall"
	if (m_Rect.y + m_Rect.h >= m_LevelBoundingBox.y + m_LevelBoundingBox.h + 25)
	{
		m_Dx = 0;
		m_Dy = 0;
		
		_UI.ReduceLife();
	}
	// If Ball hits Right wall
	else if (m_Rect.x + m_Rect.w >= m_LevelBoundingBox.x + m_LevelBoundingBox.w && GetDx() > 0)
	{
		SetDx(-GetDx()); // Bounce Left
		hasHit = true;
	}
	// If Ball hits Left wall
	else if (m_Rect.x <= m_LevelBoundingBox.x && GetDx() < 0)
	{
		SetDx(-GetDx()); // Bounce Right
		hasHit = true;
	}
	// If Ball hits Top wall
	else if (m_Rect.y <= m_LevelBoundingBox.y && GetDy() < 0)
	{
		SetDy(-GetDy()); // Bounce Down
		hasHit = true;
	}

	if (hasHit)
	{
		Mix_Chunk* temp = _Level.GetHitSound();
		AssetManager::PlaySound(temp);
	}

	// -----------------------------------------------------------------------

	// Update Ball Position
	m_Rect.x += m_Dx;
	m_Rect.y += m_Dy;

}

void Ball::OnRender()
{
	AssetManager::RenderTexture(m_Texture, NULL, &m_Rect);
}



void Ball::ResetPosition()
{
	m_Rect.x = m_LevelBoundingBox.x + (m_LevelBoundingBox.w / 2) - (m_Rect.w / 2);
	m_Rect.y = m_LevelBoundingBox.y + (m_LevelBoundingBox.h / 2) - (m_Rect.h / 2);

	// Create a random number , fast and dirty..
	std::mt19937 rng(std::random_device{}()); // Create a random number generator
	std::uniform_int_distribution<int> dist(1, 100); // Create a distribution between 1 and 100

	int random_number = dist(rng);
	random_number = random_number % 4;
	switch (random_number)
	{
	case 0:
		m_Dx = 3;
		m_Dy = -5;
		break;
	case 1:
		m_Dx = -3;
		m_Dy = -4;
		break;
	case 2:
		m_Dx = 5;
		m_Dy = -4;
		break;
	case 3:
		m_Dx = -3;
		m_Dy = -5;
	default:
		break;
	}
}

