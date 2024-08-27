#pragma once

#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "Scene.h"
#include "RenderWrapper.h"
#include "AudioManager.h"

#include "Scene_Pong.h"
#include "Scene_Menu.h"
#include "Scene_Asteroids.h"

#include <SDL.h>

#include <iostream>

enum Screens {
	SCREEN_TITLE,
	SCREEN_PONG,
	SCREEN_ASTEROIDS
};

class SceneManager
{
public:
	SceneManager(std::shared_ptr<RenderWrapper> renderWrap, ScreenInfo ScreenInformation, std::shared_ptr<AudioManager> audioManager);
	~SceneManager();

	void Render();
	void Update(float DeltaTime, SDL_Event Event);

	void ChangeScreen(Screens NewScreen);

	void SetThisReference(std::shared_ptr<SceneManager> SceneMan);

private:
	std::shared_ptr<RenderWrapper> m_RenderWrapper;
	std::shared_ptr<AudioManager> m_AudioManager;

	Scene* m_CurrentScene = nullptr;
	Screens m_CurrentSceneState;

	std::shared_ptr<SceneManager> m_SceneManager;
	ScreenInfo m_ScreenInfo;
};


#endif // !_SCENEMANAGER_H
