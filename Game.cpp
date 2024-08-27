#include "Game.h"

Game::Game()
{
    if (!InitSDL())
        std::cout << "ERROR: SDL Libraries Failed To Initialise. " << std::endl;

    if (!CreateWindow())
        std::cout << "ERROR: SDL Failed to create a window. " << std::endl;

    if (!InitialiseRendering())
        std::cout << "ERROR: SDL Failed to create a renderer. " << std::endl;

    if (!InitialiseSceneManagement())
        std::cout << "ERROR: Failed to create a Scene Manager. " << std::endl;
}

Game::~Game()
{
    CloseSDL();
}

void Game::Run()
{
    // Get the oldTime
    m_OldTime = SDL_GetTicks();

    // Flag to Check if User Wants to Quit.
    bool Quit = false;

    // Game Loop
    while (!Quit)
    {
        // Update First, Physics, AI, Transformations etc.
        Quit = Update();

        // Render Everything at its new location.
        Render();
    }
}

bool Game::Update()
{
    // Initialise a new SDL_Event Handler.
    SDL_Event Event;

    // Create a current Time
    Uint32 NewTime = SDL_GetTicks();

    // Check for new events.
    SDL_PollEvent(&Event);

    // Assign new Event to Input Handler


    // Handle the current Event Queue.
    switch (Event.type)
    {
    // Click the 'X' to quit	
    case SDL_QUIT:
        return true;
        break;
    default:
        break;
    }

    // Update SceneManager, this will pass updates to SceneObjects, GUI, Etc.
    m_SceneManager->Update((float)(NewTime - m_OldTime) / 1000.0f, Event);

    // SET OLD TIME TO NEW TIME
    m_OldTime = NewTime;

    return false;
}

void Game::Render()
{
    // Clear the current Buffer, ready to load new Pixel Information onto the buffer.
    SDL_SetRenderDrawColor(g_RenderWrapper->Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(g_RenderWrapper->Renderer);

    // Call Render On SceneManager. This passes the render call down the chain to SceneObjects, GUI, Etc.
    m_SceneManager->Render();
    
    // Render the Back Buffer To the screen.
    SDL_RenderPresent(g_RenderWrapper->Renderer);
}

bool Game::InitSDL()
{
    // Initialise SDL Library (Renderering / Events)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        std::cout << "SDL did not intialise. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialise SDL_Image Loading. 
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cout << "SDL_Image PNG Loading did not initialise. Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialise SDL_TTF Library (Fonts / Text)
    TTF_Init();
    
    // Initialise SDL_Mixer Library (Audio)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_Mixer did not initialise. Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Create Audio Manager
    g_AudioManager = std::make_shared<AudioManager>();

    return true;
}

bool Game::CreateWindow()
{
    // Initialise a New Window, With a name and width/height.
    m_Window = SDL_CreateWindow(m_ScreenStats.ScreenName.c_str(),
               SDL_WINDOWPOS_UNDEFINED,
               SDL_WINDOWPOS_UNDEFINED,
               m_ScreenStats.ScreenWidth,
               m_ScreenStats.ScreenHeight,
               SDL_WINDOW_SHOWN);

    // Check If the Window was successfully created. 
    if (m_Window == nullptr)
    {
        std::cout << "SDL Failed to create a window. Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Game::InitialiseRendering()
{
    // Initialise a new renderer.
    SDL_Renderer* tempRenderer = SDL_CreateRenderer(m_Window, -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Check if the renderer was successfully Initialised. 
    if (tempRenderer == nullptr)
    {
        std::cout << "SDL could not initialise a new renderer. Error: " << SDL_GetError() << std::endl; 
        return false;
    }

    // Initialise Texture Manager
    std::shared_ptr<TextureManager> TempTexManager = std::make_shared<TextureManager>(tempRenderer);

    // Check Texture Manager Successfully Created
    if (TempTexManager == nullptr)
    {
        std::cout << "Failed to Initialise a Texture Manager!" << std::endl;
        return false;
    }

    // Initialise Font Manager
    std::shared_ptr<FontManager> TempFontManager = std::make_shared<FontManager>(tempRenderer);

    // Check Font Manager Successfully Created
    if (TempFontManager == nullptr)
    {
        std::cout << "Failed to Initialise a Font Manager!" << std::endl;
        return false;
    }

    // Initialise Render Wrapper
    g_RenderWrapper = std::make_shared<RenderWrapper>(tempRenderer , TempTexManager, TempFontManager);

    return true;
}

bool Game::InitialiseSceneManagement()
{
    m_SceneManager = std::make_shared<SceneManager>(g_RenderWrapper, m_ScreenStats, g_AudioManager);
    m_SceneManager->SetThisReference(m_SceneManager);
    m_SceneManager->ChangeScreen(SCREEN_TITLE);
    return true;
}



void Game::CloseSDL()
{
    // delete any pointers
    SDL_DestroyWindow(m_Window);

    SDL_Quit();
}
