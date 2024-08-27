#pragma once
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "ComponentTypes.h"
#include "GameObject.h"

#include <vector>
#include <iostream> 

class Component
{
public:
	Component(GameObject* OwningGO, int UpdateOrder = 100, ComponentTypes type = COMPTYPE_None); // Default Constructor, Update order is 100 unless Specified otherwise.
	virtual ~Component() { m_ParentObject = nullptr; } // Destructor, Should be overriddon by the inherited Component Type.

	virtual void Update(float DeltaTime) {} // Update Function, Should be overiddon by inherited Component Type.

	int GetUpdateOrder() const { return m_UpdateOrder; } // Return the Update Order. Will determine when its updated compared to other components.
	ComponentTypes GetCompType() { return m_CompType; }
	GameObject* GetParentObject();

private:
	ComponentTypes m_CompType;
	int m_UpdateOrder;

protected:
	GameObject* m_ParentObject;
};

int Partition(std::vector<Component*>& array, int start, int end);

namespace Components
{
	void SortComponents(std::vector<Component*>& array, int start, int end);
}

#endif // !_COMPONENT_H
