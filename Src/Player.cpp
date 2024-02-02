#include "Player.h"

#include "AssetManager.h"


Player::Player(const SDL_Rect& _LevelBoundingBox, std::string _TexturePath)
	: Object(), m_Dx(0), m_LevelBoundingBox(_LevelBoundingBox)
{
	// Load assets
	LoadTexture(_TexturePath);

	// Set width and height
	AssetManager::SetSizeFromTexure(m_Texture, m_Rect);

	// Setting the paddle/Player in the right position
	m_Rect.x = _LevelBoundingBox.x + (_LevelBoundingBox.w / 2) - (m_Rect.w / 2);
	m_Rect.y = _LevelBoundingBox.h + _LevelBoundingBox.y - m_Rect.h;

}

Player::~Player()
{
}

void Player::OnUpdate(Ball& ball)
{
	// ------------------------ Handles Player<->Ball collisions ------------------------

	SDL_Rect ballRect = ball.GetRect();
	int padding = 0;

	// If ball travels DOWN, check for collision 
	if (ball.GetDy() > 0)
	{
		if (SDL_HasIntersection(&m_Rect, &ballRect))
		{
			ResloveCollision(ball);
			AssetManager::PlaySound(ball.GetSound());
		}
	}

	// ------------------------- Handles Player Movement ----------------------------

	// If player stationary, no need to update Player Movement
	if (m_Dx == 0)
		return;

	// if player can move right, move it
	if (m_Dx > 0 && CanAccelerateRight())
	{
		m_Rect.x += m_Dx;
		return;
	}
	// if player can move left, move it
	if (m_Dx < 0 && CanAccelerateLeft())
	{
		m_Rect.x += m_Dx;
		return;
	}
}

void Player::OnRender()
{
	AssetManager::RenderTexture(m_Texture, NULL, &m_Rect);
}


bool Player::CanAccelerateLeft() const
{
	// Check if Player has reached Left wall
	if (m_Rect.x <= m_LevelBoundingBox.x)
	{
		return false; // if reached left wall, can't accelerate
	}
	else
	{
		return true;  // Player is free accelerate
	}

}

bool Player::CanAccelerateRight() const
{
	// Check if Player has reached Right wall
	if ((m_Rect.x + m_Rect.w) >= (m_LevelBoundingBox.x + m_LevelBoundingBox.w))
	{
		return false; // if reached right wall, can't accelerate
	}
	else
	{
		return true;  // Player is free accelerate
	}
}

void Player::ResloveCollision(Ball& _Ball)
{
	// Checks the Intersection of the Ball with the Brick
	// by each vertex, so we can set the new direction of the ball

	SDL_Rect ball = _Ball.GetRect();

	int yIntersection;
	int xIntersection;

	// Ball is going UP
	if (_Ball.GetDy() < 0)
	{
		yIntersection = (m_Rect.y + m_Rect.h) - ball.y; // Its the same in both Left and Right case

		// Ball is going UP and RIGHT
		if (_Ball.GetDx() > 0)
		{
			xIntersection = (ball.x + ball.w) - m_Rect.x; // Checking Lower Left VERTEX
		}
		else // Ball going UP LEFT
		{
			xIntersection = (m_Rect.x + m_Rect.w) - ball.x; // Checking Lower Right VERTEX
		}
	}
	else // Ball is going DOWN
	{
		yIntersection = (ball.y + ball.h) - m_Rect.y; // Its the same in both Left and Right case
		// Ball going DOWN and RIGHT
		if (_Ball.GetDx() > 0)
		{
			xIntersection = (ball.x + ball.w) - m_Rect.x; // Checking Upper Left VERTEX
		}
		else // Ball going DOWN and LEFT
		{
			xIntersection = (ball.y + ball.h) - m_Rect.y; // Checking Upper Rigth VERTEX
		}
	}

	// Check which intersection is bigger, change OPOSITE axiss' direction
	if (yIntersection > xIntersection) 
	{
		_Ball.SetDx(_Ball.GetDx() * -1); // Change RIGHT/LEFT direction
		// Ball plays sound
		return;
	}
	else
	{
		_Ball.SetDy(_Ball.GetDy() * -1); // Change UP/DOWN direction
		// Ball plays sound
		return;
	}
}

