#pragma once

#ifndef _GAME_H
#define _GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include <iostream>

#include "ScreenInformation.h"
#include "SceneManager.h"
#include "RenderWrapper.h"
#include "AudioManager.h"
#include "InputHandler.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	bool InitSDL();
	bool CreateWindow();
	bool InitialiseRendering();
	bool InitialiseSceneManagement();

	void CloseSDL();

	bool Update();
	void Render();

private:
	Uint32 m_OldTime;

	std::shared_ptr<RenderWrapper> g_RenderWrapper;
	std::shared_ptr<AudioManager> g_AudioManager;

	SDL_Window* m_Window;
	ScreenInfo m_ScreenStats;

	std::shared_ptr<SceneManager> m_SceneManager;
};


#endif // !_GAME_H