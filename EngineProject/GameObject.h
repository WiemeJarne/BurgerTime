#pragma once
#include <memory>
#include <vector>
#include "Transform.h"
#include "Component.h"
#include "Scene.h"

namespace dae
{
	class GameObject final
	{
	public:
		GameObject(Scene* pScene);

		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = default;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = default;

		virtual void Update();
		virtual void Render() const;
		virtual void RenderImGui();

		void SetParent(GameObject* parent, bool keepWorldPos);
		GameObject* GetParent() const { return m_Parent; }

		size_t GetChildCount() const { return m_Children.size(); }
		dae::GameObject* GetChildAt(int index) const { return m_Children[index].get(); }

		void SetLocalPosition(float x, float y);
		const glm::vec2& GetLocalPos();

		void SetWorldPosition(float x, float y);
		const glm::vec2& GetWorldPos();
		void UpdateWorldPos();
		
		void AddComponent(std::unique_ptr<Component> component);
		
		template<typename ComponentType>
		void RemoveComponent()
		{
			for (const auto& pComponent : m_Components)
			{
				if (dynamic_cast<ComponentType*>(pComponent.get()))
				{
					m_ComponentToDeletePtrs.push_back(pComponent.get());
					return;
				}
			}
		}

		template<typename ComponentType>
		bool HasComponent() const
		{
			for (const auto& pComponent : m_Components)
			{
				if (dynamic_cast<ComponentType*>(pComponent.get()))
					return true;
			}

			return false;
		}

		template<typename ComponentType>
		ComponentType* GetComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (auto componentPtr = dynamic_cast<ComponentType*>(component.get()))
					return componentPtr;
			}

			return nullptr;
		}

		void EraseComponentsMarkedForDelete();

	private:
		std::vector<std::unique_ptr<Component>> m_Components;
		std::vector<Component*> m_ComponentToDeletePtrs;
		std::vector<std::shared_ptr<GameObject>> m_Children;
		GameObject* m_Parent{ nullptr };
		Transform m_LocalTransform{};
		Transform m_WorldTransform{};
		bool m_UpdateWorldPos{};
		Scene* m_pScene{};
	
		void AddChild(std::shared_ptr<dae::GameObject> pChild);
		std::shared_ptr<dae::GameObject> RemoveChild(dae::GameObject* pChild);
	};
}