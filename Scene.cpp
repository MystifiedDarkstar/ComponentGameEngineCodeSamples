#include "Scene.h"
#include "CollisionChecks.h"

Scene::Scene(std::shared_ptr<RenderWrapper> RenderWrap, std::shared_ptr<SceneManager> SceneMan, ScreenInfo ScreenInformation, std::shared_ptr<AudioManager> AudioManager) :
	m_RenderWrapper(RenderWrap),
	m_SceneManager(SceneMan),
	m_ScreenInfo(ScreenInformation),
	m_AudioManager(AudioManager)
{
	m_GameObjects.reserve(12000);
}

void Scene::Render()
{
	for (auto sprRenderer : m_SpriteRenderers)
	{
		sprRenderer->Render();
	}

	for (auto GUIText : m_GUITexts)
	{
		GUIText->Render();
	}
}

void Scene::Update(float DeltaTime, SDL_Event Event)
{
	// Update Mouse Position
	SDL_GetMouseState(&m_XMousePos, &m_YMousePos);

	// Update Objects
	for (auto GO : m_GameObjects)
	{
		GO->Update(DeltaTime);
	}
	// Check Collisions
	// Set all Hit Objects Nullptr 
	
	for (auto collider : m_BoxColliders)
	{
		collider->SetHitObject(nullptr);
	}

	for (auto collider : m_CircleColliders)
	{
		collider->SetHitObject(nullptr);
	}

	// Check Collisions and Set Hit Objects for Collisions
	// Box vs Box Colliders 
	for (size_t i = 0; i < m_BoxColliders.size(); i++)
	{
		for (size_t f = 0; f < m_BoxColliders.size(); f++) 
		{
			if (i == f) // Dont Need to Check a Collider against itself
				continue;
			else if (f < i) // Collision 1 vs Collision 2 Already Checked
				continue;
			else
			{
				// Check If Collision Occured
				if (CollisionCheck::BoxVSBoxCollision(m_BoxColliders[i]->GetBoxCollisionRect(), m_BoxColliders[f]->GetBoxCollisionRect()))
				{
					m_BoxColliders[i]->SetHitObject(m_BoxColliders[f]->GetParentObject());
					m_BoxColliders[f]->SetHitObject(m_BoxColliders[i]->GetParentObject());
				}
			}
		}
	}

	// Box vs Circle Colliders
	for (size_t i = 0; i < m_BoxColliders.size(); i++)
	{
		for (size_t f = 0; f < m_CircleColliders.size(); f++)
		{
			// Check If Collision Occured
			if (CollisionCheck::BoxVSCircleCollision(m_BoxColliders[i]->GetBoxCollisionRect(), m_CircleColliders[f]->GetCircleCollisionRect()))
			{
				m_BoxColliders[i]->SetHitObject(m_CircleColliders[f]->GetParentObject());
				m_CircleColliders[f]->SetHitObject(m_BoxColliders[i]->GetParentObject());

			}
		}
	}

	// Circle vs Circle Colliders
	for (size_t i = 0; i < m_CircleColliders.size(); i++)
	{
		for (size_t f = 0; f < m_CircleColliders.size(); f++)
		{
			if (i == f) // Dont Need to Check a Collider against itself
				continue;
			else if (f < i) // Collision 1 vs Collision 2 Already Checked
				continue;
			else
			{
				// Check If Collision Occured
				if (CollisionCheck::CircleVsCircleCollision(m_CircleColliders[i]->GetCircleCollisionRect(), m_CircleColliders[f]->GetCircleCollisionRect()))
				{
					m_CircleColliders[i]->SetHitObject(m_CircleColliders[f]->GetParentObject());
					m_CircleColliders[f]->SetHitObject(m_CircleColliders[i]->GetParentObject());
				}
				// Collision Didnt Occur
				else
				{
					m_CircleColliders[i]->SetHitObject(nullptr);
					m_CircleColliders[f]->SetHitObject(nullptr);
				}
			}
		}
	}


	// Scene Specific Update
	UpdateScene(DeltaTime, Event);

}

GameObject* Scene::CreateGameObject()
{
    GameObject* TempObject = new GameObject(this);
	m_GameObjects.push_back(TempObject);
	return TempObject;

	// OPTIMISATION WARNING :: THIER IS A POINTER LEFT DANGLING HERE.
}

void Scene::RemoveGameObject(GameObject* GameObjectREF)
{
	for (int i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i]->GetID() == GameObjectREF->GetID())
		{
			m_GameObjects.erase(m_GameObjects.begin() + i, m_GameObjects.begin() + i + 1);
		}
	}
}

SpriteRenderer* Scene::Create2DSpriteRenderer(GameObject* OwningGO, SDL_Texture* SpriteTexture, RGBValue ColourMod, uint8_t OpacityValue)
{
	SpriteRenderer* TempSR = new SpriteRenderer(OwningGO, m_RenderWrapper, SpriteTexture, ColourMod, OpacityValue);
	m_SpriteRenderers.push_back(TempSR);
	return TempSR;

	// OPTIMISATION WARNING :: THIER IS A POINTER LEFT DANGLING HERE.
}

void Scene::RemoveSpriteRenderer(int GameObjectID)
{
	for (int i = 0; i < m_SpriteRenderers.size(); i++)
	{
		if (m_SpriteRenderers[i]->GetParentObject()->GetID() == GameObjectID)
		{
			m_SpriteRenderers[i]->UpdateTexture(nullptr);
			m_SpriteRenderers.erase(m_SpriteRenderers.begin() + i, m_SpriteRenderers.begin() + i + 1);
		}
	}
}

BoxColliderComponent* Scene::Create2DBoxCollider(GameObject* OwningGO)
{
	// create new collider
	BoxColliderComponent* TempCollider = new BoxColliderComponent(OwningGO, BoxRect(OwningGO->GetTransform()->Position.x, 
																					OwningGO->GetTransform()->Position.y, 
																					OwningGO->GetTransform()->Size.x, 
																					OwningGO->GetTransform()->Size.y )); 


	m_BoxColliders.push_back(TempCollider); // add it to scene colliders
	return TempCollider;

	// OPTIMISATION WARNING :: THIER IS A POINTER LEFT DANGLING HERE.
}

CircleColliderComponent* Scene::Create2DCircleCollider(GameObject* OwningGO)
{
	// create new collider
	CircleColliderComponent* TempCollider = new CircleColliderComponent(OwningGO, CircleRect(OwningGO->GetTransform()->Position.x,
																							 OwningGO->GetTransform()->Position.y,
																							 OwningGO->GetTransform()->Size.x / 2));

	m_CircleColliders.push_back(TempCollider); // add it to scene colliders
	return TempCollider;

	// OPTIMISATION WARNING :: THIER IS A POINTER LEFT DANGLING HERE.
}

GUIText* Scene::CreateGUIText(GameObject* OwningGO, TTF_Font* Font, RGBValue FontColour, uint8_t OpacityValue)
{
	GUIText* TempGUIText = new GUIText(OwningGO, m_RenderWrapper, Font, RGBValue{255, 255, 255}, 255);

	m_GUITexts.push_back(TempGUIText);
	return TempGUIText;
}

void Scene::Remove2DBoxCollider(int GameObjectID)
{
	for (int i = 0; i < m_BoxColliders.size(); i++)
	{
		if (m_BoxColliders[i]->GetParentObject()->GetID() == GameObjectID)
		{
			m_BoxColliders.erase(m_BoxColliders.begin() + i, m_BoxColliders.begin() + i + 1);
		}
	}
}

void Scene::Remove2DCircleCollider(int GameObjectID)
{
	for (int i = 0; i < m_CircleColliders.size(); i++)
	{
		if (m_CircleColliders[i]->GetParentObject()->GetID() == GameObjectID)
		{
			m_CircleColliders.erase(m_CircleColliders.begin() + i, m_CircleColliders.begin() + i + 1);
		}
	}
}

void Scene::RemoveGUIText(int GameObjectID)
{
	for (int i = 0; i < m_GUITexts.size(); i++)
	{
		if (m_GUITexts[i]->GetParentObject()->GetID() == GameObjectID)
		{
			m_GUITexts.erase(m_GUITexts.begin() + i, m_GUITexts.begin() + i + 1);
		}
	}
}

Vector2D Scene::GetMousePosition()
{
	Vector2D MousePos = { (float)m_XMousePos , (float)m_YMousePos };
	return MousePos;
}

void Scene::PlaySound(AudioListener* AudioListener, int Channel, bool Looping)
{
	if (Looping)
		AudioListener->PlayFile(Channel, -1);
	else
		AudioListener->PlayFile(Channel, 0);
}

void Scene::SetVolume(AudioListener* AudioListener, int Volume)
{
	AudioListener->SetVolume(Volume);
}
