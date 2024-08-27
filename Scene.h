#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "RenderWrapper.h"
#include "AudioManager.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "MovementComponent.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "AsteroidComponent.h"
#include "ScreenWrapperComponent.h"
#include "ShipInputComponent.h"
#include "PlayerHealthSystem.h"
#include "AsteroidsScoreSystem.h"
#include "GUIText.h"
#include "GUIButton.h"

#include <vector>
#include <iostream>

#define AUDIO_MUSICCHANNEL -1 
#define AUDIO_SFXCHANNEL1 1
#define AUDIO_SFXCHANNEL2 2
#define AUDIO_SFXCHANNEL3 3

class Scene
{
public:
	Scene(std::shared_ptr<RenderWrapper> RenderWrap, std::shared_ptr<class SceneManager> SceneMan, ScreenInfo ScreenInformation, std::shared_ptr<AudioManager> AudioManager);
	virtual ~Scene() {}

	void Render();
	void Update(float DeltaTime, SDL_Event Event);

	GameObject* CreateGameObject();
	void RemoveGameObject(GameObject* GameObject);

	SpriteRenderer* Create2DSpriteRenderer(GameObject* OwningGO, SDL_Texture* SpriteTexture, RGBValue ColourMod, uint8_t OpacityValue);
	BoxColliderComponent* Create2DBoxCollider(GameObject* OwningGO);
	CircleColliderComponent* Create2DCircleCollider(GameObject* OwningGO);
	GUIText* CreateGUIText(GameObject* OwningGO, TTF_Font* Font, RGBValue FontColour, uint8_t OpacityValue);

	void RemoveSpriteRenderer(int GameObjectID);
	void Remove2DBoxCollider(int GameObjectID);
	void Remove2DCircleCollider(int GameObjectID);
	void RemoveGUIText(int GameObjectID);

	Vector2D GetMousePosition();
	ScreenInfo GetScreenStats() { return m_ScreenInfo; }

	void PlaySound(AudioListener* AudioListener, int Channel, bool Looping);
	void SetVolume(AudioListener* AudioListener, int Volume);
	std::shared_ptr<AudioManager> GetAudioManager() { return m_AudioManager; }

private:
	virtual void SetUpScene() {}
	virtual void UpdateScene(float DeltaTime, SDL_Event Event) {}

private:
	int m_XMousePos = 0, m_YMousePos = 0;

	std::vector<GameObject*> m_GameObjects; 
	std::vector<SpriteRenderer*> m_SpriteRenderers;
	std::vector<BoxColliderComponent*> m_BoxColliders;
	std::vector<CircleColliderComponent*> m_CircleColliders;
	std::vector<GUIText*> m_GUITexts;
	ScreenInfo m_ScreenInfo;

protected:
	std::shared_ptr<RenderWrapper> m_RenderWrapper;
	std::shared_ptr<AudioManager> m_AudioManager;
	std::shared_ptr<class SceneManager> m_SceneManager;
};

#endif