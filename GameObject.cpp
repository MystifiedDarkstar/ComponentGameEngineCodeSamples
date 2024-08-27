#include "GameObject.h"
#include "Component.h"
#include "Scene.h"

std::atomic<int> GameObject::s_ID;

GameObject::GameObject(Scene* OwningScene) :
	m_OwningScene(OwningScene),
	m_ID(s_ID++),
	m_HorizontalAlignment(HA_Left),
	m_VerticalAlignment(VA_Top)
{
	m_Transform = std::make_shared<Transform>();
	m_Transform->Size.x = 32;
	m_Transform->Size.y = 32;
	SetPosition(Vector2D{ 0,0 });
}

GameObject::~GameObject()
{
	for (auto comp : m_Components)
	{
		RemoveComponent(comp->GetCompType());
	}
	GetParentScene()->RemoveGameObject(this);
}

void GameObject::Update(float DeltaTime)
{
	if (m_IsActive)
	{
		UpdateComponents(DeltaTime);
		UpdateGameObject(DeltaTime);
	}

	if (m_PendingDestroy)
		this->~GameObject();
}

Scene* GameObject::GetParentScene()
{
	return m_OwningScene;
}

void GameObject::AddComponent(Component* component)
{
	// Add new component to vector
	m_Components.push_back(component); 

	// Sort components in order of UpdateOrder. Lowest order to 0 first.
	// This is because some components like a Camera/Collision might want to update after movement has taken place.
	Components::SortComponents(m_Components, 0 , (int)m_Components.size() - 1); 
}

Component* GameObject::GetComponent(ComponentTypes Type)
{
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->GetCompType() == Type)
		{
			return m_Components[i];
		}
	}
}

void GameObject::RemoveComponent(ComponentTypes Type)
{
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->GetCompType() == Type)
		{
			m_Components.erase(m_Components.begin() + i, m_Components.begin() + i + 1);
			if (Type == COMPTYPE_TwoDSpriteRenderer)
			{
				m_OwningScene->RemoveSpriteRenderer(m_ID);
			}
			else if (Type == COMPTYPE_BoxCollider)
			{
				m_OwningScene->Remove2DBoxCollider(m_ID);
			}
		}
	}
}

void GameObject::SetPosition(Vector2D NewPosition)
{
	Vector2D TempPosition = {0,0};

	switch (m_HorizontalAlignment)
	{
	case HA_Left:
		TempPosition.x = 0;
		break;
	case HA_Centre:
		TempPosition.x = (GetParentScene()->GetScreenStats().ScreenWidth / 2) - (m_Transform->Size.x / 2);
		break;
	case HA_Right:
		TempPosition.x = GetParentScene()->GetScreenStats().ScreenWidth - m_Transform->Size.x;
		break;
	default:
		break;
	}

	switch (m_VerticalAlignment)
	{
	case VA_Top:
		TempPosition.y = 0;
		break;
	case VA_Centre:
		TempPosition.y = (GetParentScene()->GetScreenStats().ScreenHeight / 2) - (m_Transform->Size.y / 2);
		break;
	case VA_Bottom:
		TempPosition.y = GetParentScene()->GetScreenStats().ScreenHeight - m_Transform->Size.y;
		break;
	default:
		break;
	}

	m_Transform->Position = ( TempPosition += NewPosition );
}

void GameObject::SetSize(Vector2D NewSize)
{
	m_Transform->Size = NewSize;
}

void GameObject::SetTag(Tags Tag)
{
	m_Tag = Tag;
}

bool GameObject::CompareTag(Tags Tag)
{
	if (m_Tag == Tag)
		return true;
	else
		return false;
}

void GameObject::Destroy()
{
	m_PendingDestroy = true;
}

void GameObject::SetHorizontalAlignment(HorizontalAlignments HA)
{
	m_HorizontalAlignment = HA;
	SetPosition(m_Transform->Position);
}

void GameObject::SetVerticalAlignment(VerticalAlignments VA)
{
	m_VerticalAlignment = VA;
	SetPosition(m_Transform->Position);

}

void GameObject::UpdateComponents(float DeltaTime)
{
	for (auto comp : m_Components)
	{
		comp->Update(DeltaTime);
	}
}
