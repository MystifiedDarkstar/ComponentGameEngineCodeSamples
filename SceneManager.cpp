#include "SceneManager.h"

SceneManager::SceneManager(std::shared_ptr<RenderWrapper> renderWrap, ScreenInfo ScreenInformation, std::shared_ptr<AudioManager> audioManager) :
	m_RenderWrapper(renderWrap),
	m_ScreenInfo(ScreenInformation),
	m_AudioManager(audioManager)
{

}

SceneManager::~SceneManager()
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
}

void SceneManager::Render()
{
	m_CurrentScene->Render();
}

void SceneManager::Update(float DeltaTime, SDL_Event Event)
{
	m_CurrentScene->Update(DeltaTime, Event);
}

void SceneManager::ChangeScreen(Screens NewScreen)
{
	m_AudioManager->ResetAllAudios();

	// If old scene exists, Clear It
	if (m_CurrentScene != nullptr)
	{
		delete m_CurrentScene;
		m_CurrentScene = nullptr;
	}

	Scene* TempScene;
	// Find new screen Type
	switch (NewScreen)
	{
	case SCREEN_TITLE:
		TempScene = (Scene*) new Scene_Menu(m_RenderWrapper, m_SceneManager, m_ScreenInfo, m_AudioManager);
		m_CurrentScene = TempScene;
		m_CurrentSceneState = NewScreen;
		TempScene = nullptr;
		break;
	case SCREEN_PONG:
		TempScene = (Scene*) new Scene_Pong(m_RenderWrapper, m_SceneManager, m_ScreenInfo, m_AudioManager);
		m_CurrentScene = TempScene;
		m_CurrentSceneState = NewScreen;
		TempScene = nullptr;
		break;
	case SCREEN_ASTEROIDS:
		TempScene = (Scene*) new Scene_Asteroids(m_RenderWrapper, m_SceneManager, m_ScreenInfo, m_AudioManager);
		m_CurrentScene = TempScene;
		m_CurrentSceneState = NewScreen;
		TempScene = nullptr;
		break;
	default:
		std::cout << "Error: Scene does not exist. " << std::endl;
		break;
	}
	//delete TempScene;
	//TempScene = nullptr;
}

void SceneManager::SetThisReference(std::shared_ptr<SceneManager> SceneMan)
{
	m_SceneManager = SceneMan;
}
