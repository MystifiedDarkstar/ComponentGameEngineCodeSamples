#pragma once

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "ComponentTypes.h"
#include "GameObjectTags.h"
#include "Transform.h"

#include "TextAlignments.h"

#include <iostream>
#include <vector>

class GameObject
{
public:
	GameObject(class Scene* OwningScene);
	~GameObject();

	void Update(float DeltaTime); // Update called from Scene. Not overridable.

	int GetID() { return m_ID; }

	Scene* GetParentScene();

	void AddComponent(class Component* component);
	Component* GetComponent(ComponentTypes Type);
	void RemoveComponent(ComponentTypes Type);

	std::shared_ptr<Transform> GetTransform() { return m_Transform; }
	void SetPosition(Vector2D NewPosition); 
	void SetSize(Vector2D NewSize);

	void SetTag(Tags Tag);
	bool CompareTag(Tags Tag);

	void Destroy();

	void SetHorizontalAlignment(HorizontalAlignments HA);
	void SetVerticalAlignment(VerticalAlignments VA);

	void SetActive(bool state) { m_IsActive = state; }
	bool IsActive() { return m_IsActive; }

	bool IsPendingDelete() { return m_PendingDestroy; }

private:
	void UpdateComponents(float DeltaTime); // Updates all components. Not overridable.
	virtual void UpdateGameObject(float DeltaTime) {} // Any GameObject Specific Update Code. overridable.

private:
	int m_ID;
	Tags m_Tag;

	Scene* m_OwningScene;

	std::shared_ptr<Transform> m_Transform;
	std::vector<class Component*> m_Components;

	HorizontalAlignments m_HorizontalAlignment;
	VerticalAlignments m_VerticalAlignment;

	bool m_PendingDestroy = false;
	bool m_IsActive = true;

protected:
	static std::atomic<int> s_ID;
};

#endif // !_GAMEOBJECT_H
