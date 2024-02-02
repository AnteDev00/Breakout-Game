#include "Level.h"
#include "AssetManager.h"

#include "tinyxml2.h"

Level::Level(const int& _ScreenWidth, const int& _ScreenHeight, const std::string& _LevelFilepath)
	: m_BrickManager(nullptr), m_BoardTexture(nullptr), m_HitSound(AssetManager::LoadSound("Assets/Sounds/WallSound.wav")), 
	m_BoundingBox({ 0, 0, 0, 0 }), m_Rect({ 0, 0, 0, 0 })
{
	// Loads m_BoardTexturePath, and all the that BrickManager neeeds (also creates BrickManager)
	if (!LoadLevel(_LevelFilepath))
	{
		std::cerr << "Failed to load level" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// This has to go in order
	m_BoardTexture = AssetManager::LoadTexturePNG(m_BoardTexturePath, Breakout::m_Renderer); // Create level texture 
	SetLevelRect(_ScreenWidth, _ScreenHeight); // From the texture and from screen size
	SetLevelBoundingBox(); // From the level rect

	// Creates bricks and positions them
	m_BrickManager->CreateLevel(m_BoundingBox);

	// Create sound
	m_HitSound = AssetManager::LoadSound("Assets/Sounds/WallSound.wav");

	std::cout << "Level Created Successfuly" << std::endl;	
}

Level::~Level()
{
	if (m_HitSound != nullptr)
	{
		Mix_FreeChunk(m_HitSound);
	}
	if (m_BoardTexture != nullptr)
	{
		SDL_DestroyTexture(m_BoardTexture);
	}
}


void Level::OnUpdate(Ball& ball, UI& _UI)
{
	m_BrickManager->OnUpdate(ball, _UI);
}

void Level::OnRender()
{	
	// Render Level 
	AssetManager::RenderTexture(m_BoardTexture, NULL, &m_Rect);	

	// Render Bricks
	m_BrickManager->OnRender();
}

bool Level::LoadLevel(const std::string& _LevelFilepath)
{

	// Delete old level, if exists
	if (m_BrickManager != nullptr)
	{
		m_BrickManager->~BrickManager();
		delete m_BrickManager;
		SDL_DestroyTexture(m_BoardTexture);
	}
	// -------------------------- Load XML File -----------------
	using namespace tinyxml2;
	XMLDocument XMLDocument;
	if (XMLDocument.LoadFile(_LevelFilepath.c_str()) != XML_SUCCESS)
	{
		std::cerr << "Failed to load level file\n";
		return false;
	}
	
	XMLElement* XMLLevel = XMLDocument.FirstChildElement("Level");
	if (XMLLevel == nullptr)
	{
		std::cerr << "Error: Level element not found.\n";
		return false;
	}
	
	XMLElement* XMLBrickTypes = XMLLevel->LastChildElement("BrickTypes");
	if (XMLBrickTypes == nullptr)
	{
		std::cerr << "Error: BrickTypes element not found.\n";
		return false;
	}
	
	XMLElement* XMLBricks = XMLLevel->LastChildElement("Bricks");
	if (XMLBricks == nullptr)
	{
		std::cerr << "Error: Bricks element not found.\n";
		return false;
	}
	
	// ------------------------ Load data ------------------------
	// Level data
	m_BoardTexturePath = XMLLevel->Attribute("BackgroundTexture");
	// Brick data
	int rowCount = atoi(XMLLevel->Attribute("RowCount"));
	int columnCount = atoi(XMLLevel->Attribute("ColumnCount"));
	int rowSpacing = atoi(XMLLevel->Attribute("RowSpacing"));
	int columnSpacing = atoi(XMLLevel->Attribute("ColumnSpacing"));
	std::string brickSequence = XMLBricks->FirstChild()->Value();

	// Securty check
	if (rowCount == 0 || columnCount == 0 || rowSpacing == 0 || columnSpacing == 0 
		|| m_BoardTexturePath.size() == 0 || brickSequence.length() == 0)
	{
		return false;
	}
	// ------------------ Pass Data to Brick manager --------------

	m_BrickManager = new BrickManager(rowCount, columnCount,
		rowSpacing, columnSpacing, brickSequence);

	// -------------------- Load BRICK TYPES ----------------------

	// Loading first BrickType
	XMLElement* XMLBrickType = XMLBrickTypes->FirstChildElement("BrickType"); 
	// Getting the number of brick types
	int TYPES_COUNT = XMLBrickTypes->ChildElementCount(); 

	// Itterating through each brick type, and storing its data in a vector
	for (int i = 0; i < TYPES_COUNT; i++)
	{
		if (!XMLBrickType)
		{
			std::cerr << "Error: " << i << ".th BrickType element not found.\n";
			return false;
		}
		SolidBrickType* brickType;

		std::string id = XMLBrickType->Attribute("Id");
		std::string texture = XMLBrickType->Attribute("Texture");
		std::string hitPoints = XMLBrickType->Attribute("HitPoints");
		std::string hitSound = XMLBrickType->Attribute("HitSound");

		if (id != "I")
		{
			std::string breakSound = XMLBrickType->Attribute("BreakSound");
			std::string breakScore = XMLBrickType->Attribute("BreakScore");
			brickType = new FragileBrickType(id, texture, hitPoints, hitSound, breakSound, breakScore);
			m_BrickManager->AddBrickType(brickType);
			XMLBrickType = XMLBrickType->NextSiblingElement();
		}
		else
		{
			brickType = new SolidBrickType(id, texture, hitPoints, hitSound);
			m_BrickManager->AddBrickType(brickType);
			XMLBrickType = XMLBrickType->NextSiblingElement();
		}
	}

	// Check if all types were created successfully
	if (m_BrickManager->GetBrickTypeCount() != TYPES_COUNT)
		return false;
	else
		return true;
}

void Level::ResetLevel() const
{
	m_BrickManager->ResetBricks();
}


void Level::SetLevelRect(const int& _ScreenWidth, const int& _ScreenHeight)
{
	// Sets Level w and h
	AssetManager::SetSizeFromTexure(m_BoardTexture, m_Rect);
	// Sets Level x and y
	m_Rect.x = (_ScreenWidth - m_Rect.w) / 2; // Insures equal Left and right padding
	m_Rect.y = (_ScreenHeight - m_Rect.h) * 20 / 100; // Insures Top small padding, Bottom big padding
}

void Level::SetLevelBoundingBox()
{
	// "Magic" number
	int wallThickness = 25;

	// Setting Levels' Bounding Box 
	m_BoundingBox.x = m_Rect.x + wallThickness; // Left padding
	m_BoundingBox.y = m_Rect.y + wallThickness; // Top padding
	m_BoundingBox.w = m_Rect.w - (2 * wallThickness); // 2 walls (Left and Right) * wall width
	m_BoundingBox.h = m_Rect.h - (1 * wallThickness); // 1 wall (Top) * wall width
}

