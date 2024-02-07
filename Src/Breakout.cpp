#include "Breakout.h"
#include "AssetManager.h"

#include "SDL_mixer.h"

// Static gloabl variables
SDL_Window* Breakout::m_Window = nullptr;
SDL_Renderer* Breakout::m_Renderer = nullptr;
SDL_Event m_Event;

Breakout::Breakout() : m_ScreenWidth(800), m_ScreenHeight(640), m_Running(false), m_Level(nullptr), m_Player(nullptr), m_Ball(nullptr), m_UI(nullptr)
{
	// Initializing SDL
	if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
		return;
	}

	// Initializing SDL_mixer
	if(SDL_Init(SDL_INIT_AUDIO) != 0)
	{
		std::cerr << "SDL_mixer initialization error: " << SDL_GetError() << std::endl;
		return;
	}
	// Opening audio device
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) 
	{
		std::cerr << "SDL_mixer error: Cannot open Audio Device: " << Mix_GetError() << std::endl;
		return;
	}

	// Creating window 
	Uint32 WindowFlags = SDL_WINDOW_SHOWN;
	m_Window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,	m_ScreenWidth, 
		m_ScreenHeight,	WindowFlags);

	// Creating renderer
	Uint32 RendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	m_Renderer = SDL_CreateRenderer(m_Window, -1, RendererFlags);

	// Securty check
	if (m_Window == nullptr || m_Renderer == nullptr)
	{
		std::cerr << "Failed to Initialize\n";
		return;
	}

	// Instantiating game objects
	m_Level = new Level(m_ScreenWidth, m_ScreenHeight ,"Bin/x64/Debug/Level1.xml");	
	m_Player = new Player(m_Level->GetBoundingBox(), "Assets/Textures/Players/Player1.png"); 
	m_Ball = new Ball(m_Level->GetBoundingBox(), "Assets/Textures/Balls/Ball1.png");
	m_UI = new UI(*m_Level, "Assets/Fonts/NES-Chimera/NES-Chimera.ttf");
	

	// If nothing fails,
	m_Running = true; // Game loop can start 
	std::cout << "Game Initialized!" << std::endl;
}


// Freeing memory we alocated 
Breakout::~Breakout() 
{
	// SDL Mixer
	Mix_CloseAudio();
	Mix_Quit();

	// SDL
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);
	SDL_Quit();

	// Objects	
	delete m_Level;
	delete m_Player;
	delete m_Ball;

	std::cout << "Game Cleaned!" << std::endl;
}


// Game loop
void Breakout::Run()  
{
	std::cout << "Game Loop started!" << std::endl;
	while (m_Running)
	{
		OnInput();
		OnUpdate();
		OnRender();
	}
}


// Deals with user input
void Breakout::OnInput()
{
	// Get the current keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	while (SDL_PollEvent(&m_Event))
	{
		// --------------Player movement----------
		// Check left arrow key
		if (keyboardState[SDL_SCANCODE_LEFT])
		{
			m_Player->SetDx(-6);
		}
		// Check right arrow key
		else if (keyboardState[SDL_SCANCODE_RIGHT])
		{
			m_Player->SetDx(6);
		}
		else
		{
			m_Player->SetDx(0);
		}
		// -------------------------------------

		// Pressing ESC - escape
		if (keyboardState[SDL_SCANCODE_ESCAPE])
		{
			m_Running = false;
		}
		// Pressing R - restart
		if (keyboardState[SDL_SCANCODE_R])
		{
			m_Ball->ResetPosition();
			m_Level->ResetLevel();
			m_UI->ResetUI();
			std::cout << "Game restarted!" << std::endl;
		}
		// Clicking X - exit
		if (m_Event.type == SDL_QUIT)
		{
			m_Running = false;
			break;
		}
	}
}


// Updates game state
void Breakout::OnUpdate()
{
	if (m_UI->GetLives() != 0)
	{
		m_Level->OnUpdate(*m_Ball, *m_UI);	// Handles Ball vs Bricks
		m_Player->OnUpdate(*m_Ball); // Handles Ball vs Player
		m_Ball->OnUpdate(*m_Level, *m_UI);			// Handles Ball vs Level
	}
	else // Game Over
	{
		// to do	
	}

}


// Renders game
void Breakout::OnRender()
{
	SDL_SetRenderDrawColor(m_Renderer, 35, 35, 40, 0); // BackGround color
	SDL_RenderClear(m_Renderer); // "Clears" the screen with a BG color
	
	m_Level->OnRender(); 
	m_Player->OnRender();
	m_Ball->OnRender();	 
	m_UI->OnRender();
	
	SDL_RenderPresent(m_Renderer); // Renders the screen
}

void Breakout::ResetLevel()
{
	//m_Level->Reset();
//	m_Player->ResetPosition();
	m_Ball->ResetPosition();
}

