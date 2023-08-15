#include "GameObject.h"
#include "ResourceManager.h"
#include "Component.h"
#include "TextureComponent.h"
#include <iostream>

dae::GameObject::GameObject(Scene* pScene)
	: m_pScene{ pScene }
{}

void dae::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
	
	for (const auto& child : m_Children)
	{
		child->Update();
	}

	if (m_UpdateWorldPos)
		UpdateWorldPos();
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_Components)
	{
		component->Render();
	}

	for (const auto& child : m_Children)
	{
		child->Render();
	}
}

void dae::GameObject::RenderImGui()
{
	for (const auto& component : m_Components)
	{
		component->RenderImGui();
	}

	for (const auto& child : m_Children)
	{
		child->RenderImGui();
	}
}

void dae::GameObject::AddComponent(std::unique_ptr<Component> component)
{
	bool componentAlreadyOnThisObject{};

	//check if there already is a component of this type on this gameObject
	for (const auto& existingComponent : m_Components)
	{
		if (typeid(*existingComponent) == typeid(*component))
		{
			componentAlreadyOnThisObject = true;
		}
	}

	//if the object is not on this gameObject add it else do nothing
	if (!componentAlreadyOnThisObject)
	{
		m_Components.emplace_back(std::move(component));
	}
}

void dae::GameObject::SetParent(dae::GameObject* parent, bool keepWorldPos)
{
	//check if the passed trough parent isn't a direct child of this gameObject
	for (const auto& child : m_Children)
	{
		if (parent == child.get())
			return;
	}

	//check if the gameObject isn't trying to assing himself as his own parent
	if (parent == this)
		return;

	//if there is no parent the local position is equal to the world position
	if (!parent)
		SetLocalPosition(GetWorldPos().x, GetWorldPos().y);
	else
	{
		if (keepWorldPos) //Updates the local pos so the world pos is kept
		{
			auto newLocalPos{ GetLocalPos() - parent->GetWorldPos() };
			SetLocalPosition(newLocalPos.x, newLocalPos.y);
		}

		m_UpdateWorldPos = true;

		for (auto& child : m_Children)
		{
			child->m_UpdateWorldPos = true;
		}
	}

	std::shared_ptr<GameObject> sharedPtr{};
	bool wasOnScene{};

	//remove this gameObject as a child from the previous parent
	if (m_Parent)
	{
		sharedPtr = m_Parent->RemoveChild(this);
	}
	else
	{
		sharedPtr = m_pScene->GetSharedPtr(this);
		wasOnScene = true;
	}

	//assign the new parent
	m_Parent = parent;

	//add this gameObject as a child of the new parent
	if (m_Parent)
	{
		m_Parent->AddChild(sharedPtr);

		if (wasOnScene)
		{
			//remove this object from the scene
			m_pScene->Remove(sharedPtr);
		}
	}
	else //if the given parent is nullptr add it to the scene
		m_pScene->QueueForAdd(sharedPtr);
}

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_LocalTransform.SetPosition(x, y);

	m_UpdateWorldPos = true;

	for (auto& child : m_Children)
	{
		child->m_UpdateWorldPos = true;
	}
}

void dae::GameObject::SetWorldPosition(float x, float y)
{
	m_WorldTransform.SetPosition(x, y);
}

const glm::vec2& dae::GameObject::GetLocalPos()
{
	return m_LocalTransform.GetPosition();
}

const glm::vec2& dae::GameObject::GetWorldPos()
{ 
	if (m_UpdateWorldPos)
		UpdateWorldPos();

	return m_WorldTransform.GetPosition();
}

void dae::GameObject::UpdateWorldPos()
{
	if (!m_Parent)
	{
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition().x, m_LocalTransform.GetPosition().y);
	}
	else if(m_Parent)
	{
		auto newWorldPos{ m_Parent->GetWorldPos() + m_LocalTransform.GetPosition()};
		m_WorldTransform.SetPosition(newWorldPos.x, newWorldPos.y);
	}

	m_UpdateWorldPos = false;
}

void dae::GameObject::EraseComponentsMarkedForDelete()
{
	for (const auto& pComponent : m_ComponentToDeletePtrs)
	{
		m_Components.erase(std::remove_if(m_Components.begin(), m_Components.end(), [&pComponent](std::unique_ptr<Component>& otherComponent) { return otherComponent.get() == pComponent; }), m_Components.end());
	}

	m_ComponentToDeletePtrs.clear();
}

void dae::GameObject::AddChild(std::shared_ptr<dae::GameObject> pChild)
{
	for (const auto& child : m_Children)
	{
		if (child == pChild)
			return;
	}

	m_Children.push_back(pChild);
}

std::shared_ptr<dae::GameObject> dae::GameObject::RemoveChild(dae::GameObject* pChild)
{
	std::shared_ptr<dae::GameObject> sharedPtrChild{};

	for (const auto& child : m_Children)
	{
		if (child.get() == pChild)
		{
			sharedPtrChild = child;
			m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
			pChild->m_Parent = nullptr;
			return sharedPtrChild;
		}
	}

	return nullptr;
}