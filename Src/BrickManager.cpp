#include "BrickManager.h"

BrickManager::BrickManager( const int& rowCount, const int& columnCount,
	const int& m_RowSpacing, const int& columnSpacing, const std::string& brickSequence)
	: m_RowCount(rowCount), m_ColumnCount(columnCount), m_RowSpacing(m_RowSpacing), m_ColumnSpacing(columnSpacing),
	m_BrickSequence(brickSequence), m_BricksBorder({ 0, 0, 0, 0 })
{

}


BrickManager::~BrickManager()
{
	for (SolidBrickType* brick : m_BrickTypes)
	{
		delete brick;
	}

	for (SolidBrick* brick : m_BrickInstances)
	{
		delete brick;
	}
}

void BrickManager::OnUpdate(Ball& _Ball, UI& _UI)
{
	// Deals with Ball-Brick collisions
	//
	SDL_Rect ballRect = _Ball.GetRect();

	int padding = 6; // Padding Above and Below the Bricks
	bool bottomBorder = ballRect.y <= m_BricksBorder.y + m_BricksBorder.h + padding;
	bool topBorder = ballRect.y + ballRect.h >= m_BricksBorder.y - padding;

	// Check if the Ball is near the Bricks 
	if (!topBorder || !bottomBorder)
	{
		return;	// If not, return
	}

	// We start the brick collision check from Front/Back based on the ball direction (for efficiency reasons)
	int startIndex = (_Ball.GetDy() < 0) ? m_ColumnCount * m_RowCount - 1 : 0;
	int endIndex = (_Ball.GetDy() < 0) ? -1 : m_ColumnCount * m_RowCount;
	int step = (_Ball.GetDy() < 0) ? -1 : 1;

	for (int i = startIndex; i != endIndex; i += step)
	{
		SolidBrick* brick = m_BrickInstances[i];
		if (brick == nullptr)
			continue;
		SDL_Rect brickRect = brick->GetRect();

		// Collision check
		if (!SDL_HasIntersection(&ballRect, &brickRect))
			continue; // If no collision continue

		// Bounce direction resolutionn
		ResolveCollision(_Ball, brick);

		// Update Brick state, based on type
		if (brick->GetId()[0] == 'I')
			brick->OnCollision(_UI);
		else
		{
			FragileBrick* fragileBrick = dynamic_cast<FragileBrick*>(brick);
			if (fragileBrick != nullptr) // if cast hasnt failed
				fragileBrick->OnCollision(_UI);
		}
	}

}

void BrickManager::OnRender()
{
	// Render the Bricks
	for (SolidBrick* brick : m_BrickInstances)
	{
		if (brick != nullptr)
		{
			brick->OnRender();
		}
	}
}


void BrickManager::CreateLevel(const SDL_Rect& _BoundingBox)
{
	if (!FilterBricks())
	{
		std::cerr << "Error: FilterBrickSequence() error\n";
		std::exit(EXIT_FAILURE);
	}
	if (!CreateBricks())
	{
		std::cerr << "Failed to create all bricks\n";
		std::exit(EXIT_FAILURE);
	}
	PositionBricks(_BoundingBox);
}

bool BrickManager::FilterBricks()
{
	std::string filteredBrickSequence = "";
	
	// Filtering the Brick Sequence
	for (int i = 0; i < m_BrickSequence.length(); i++)
	{
		// Removing anything that is not a Brick (S,M,H,I) or '_' (empty brick)
		if (std::isalpha(m_BrickSequence[i]) || m_BrickSequence[i] == '_')
		{
			filteredBrickSequence += m_BrickSequence[i];
		}
	}
	
	m_BrickSequence = filteredBrickSequence;

	if (m_BrickSequence.length() != m_RowCount * m_ColumnCount)
	{
		return false;
	}
	else
	{
		return true;
	}
}


bool BrickManager::CreateBricks()
{
	for (int i = 0; i < m_BrickSequence.length(); i++)
	{
		char BrickId = m_BrickSequence[i]; // Alias for easier reading
		int BrickIdIndex = -1;

		// Getting the index of the Brick Type
		for (int i = 0; i < m_BrickTypes.size(); i++)
		{
			char BrickTypesId = m_BrickTypes[i]->GetId()[0];
			if (BrickId == BrickTypesId)
			{
				BrickIdIndex = i;
			}
			// If we found it, exit the inner loop
			if (BrickIdIndex != -1)
			{
				break;
			}
		}
		// If Somehow we didn't find the Brick Type
		if (BrickIdIndex == -1 && BrickId != '_')
		{
			return false;
		}

		// ---------------------------- Creating bricks -------------------

		// Fragile bricks (S,M,H)
		if (BrickId != 'I' && BrickId != '_')
		{
			FragileBrickType* fragileBrickType = dynamic_cast<FragileBrickType*>(m_BrickTypes[BrickIdIndex]); // Mandatory casting
			if (fragileBrickType == nullptr)
			{
				return false; // Return if cast failed
			}
			m_BrickInstances.push_back(new FragileBrick(
				fragileBrickType->GetId(),
				fragileBrickType->GetTexturePath(),
				fragileBrickType->GetHitPoints(),
				fragileBrickType->GetHitSoundPath(),
				fragileBrickType->GetBreakSoundPath(),
				fragileBrickType->GetBreakScore()
			));
		}
		else if (BrickId == '_') // Empty Brick
		{
			m_BrickInstances.push_back(new SolidBrick("_"));
			continue;
		}
		else // BrickId == 'I', aka Impenetrable brick
		{
			SolidBrickType* solidBrickType = m_BrickTypes[BrickIdIndex]; // alias for easier reading
			m_BrickInstances.push_back(new SolidBrick(
				solidBrickType->GetId(),
				solidBrickType->GetTexturePath(),
				solidBrickType->GetHitPoints(),
				solidBrickType->GetHitSoundPath()
			));
		}
	}

	// Security check
	if (m_BrickInstances.size() != m_RowCount * m_ColumnCount)
		return false;
	else
		return true;
}

void BrickManager::PositionBricks(const SDL_Rect& _BoundingBox)
{
	// --------------------- Positioning Bricks ----------------------------
	// 
	// Starting position of the bricks
	int startingYpoint = _BoundingBox.h / 3;// Top padding 
	int startingXpoint = _BoundingBox.x; // Left padding 

	// Bricks x y info
	int x = startingXpoint;
	int y = startingYpoint;

	// Brick Info
	int brickWidth = m_BrickInstances[0]->GetWidth();
	int brickHeight = m_BrickInstances[0]->GetHeigth();

	// Sets the position of all bricks
	int index;
	for (int currRow = 0; currRow < m_RowCount; currRow++)
	{
		for (int currCol = 0; currCol < m_ColumnCount; currCol++)
		{
			index = currCol + (m_ColumnCount * currRow); 

			if (m_BrickInstances[index]->GetId()[0] == m_BrickSequence[index])
			{
				m_BrickInstances[index]->SetPosition(x, y);
				x += brickWidth + m_ColumnSpacing;
			}
			else
			{
				x += brickWidth + m_ColumnSpacing;
			}
		}
		y += brickHeight + m_RowSpacing; // When we are done with a row, change y position
		x = startingXpoint;				// and reset x position
	}

	// Calculate BricksBorder for Collision detection

	m_BricksBorder.x = m_BrickInstances[0]->GetX();
	m_BricksBorder.y = m_BrickInstances[0]->GetY();

	m_BricksBorder.w = _BoundingBox.w;
	m_BricksBorder.h = (m_BrickInstances[0]->GetHeigth() * m_RowCount) + (m_RowCount - 1) * m_RowSpacing;

}

void BrickManager::ResolveCollision(Ball& _Ball, SolidBrick* _Brick)
{

	// Checks the Intersection of the Ball with the Brick
	// by each vertex, so we can set new ball direction after collision

	//for (SolidBrick* brick : m_BrickInstances)
	//{
		SDL_Rect brick = _Brick->GetRect();
		SDL_Rect ball = _Ball.GetRect();

		int yIntersection;
		int xIntersection;

		// 4 possible ball directions

		if (_Ball.GetDy() < 0) // Ball is going UP 
		{
			yIntersection = (brick.y + brick.h) - ball.y; // Same in both directions

			// UP and RIGHT
			if (_Ball.GetDx() > 0)
			{
				xIntersection = (ball.x + ball.w) - brick.x; // Lower Left VERTEX
			}
			else // UP and LEFT
			{
				xIntersection = (brick.x + brick.w) - ball.x; // Lower Right VERTEX
			}
		}
		else // Ball is going DOWN
		{
			yIntersection = (ball.y + ball.h) - brick.y; // Same in both directions
			// DOWN and RIGHT
			if (_Ball.GetDx() > 0)
			{
				xIntersection = (ball.x + ball.w) - brick.x; // Upper Left VERTEX
			}
			else // DOWN and LEFT
			{
				xIntersection = (ball.y + ball.h) - brick.y; // Upper Rigth VERTEX
			}
		}

		// Check which intersection is bigger, change OPOSITE axiss' direction
		if (yIntersection > xIntersection)
		{
			_Ball.SetDx(_Ball.GetDx() * -1); // Change Left/Right direction
			return;
		}
		else
		{
			_Ball.SetDy(_Ball.GetDy() * -1); // Change Up/Down direction
			return;
		}
	//}

}

void BrickManager::ResetBricks()
{
	for (SolidBrick* brick : m_BrickInstances)
	{
		if (brick != nullptr)
		{
			brick->Reset(m_BrickTypes);
		}
	}

}


