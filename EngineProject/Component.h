#pragma once

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		Component(dae::GameObject* owner) : m_pOwner{ owner } {};

		virtual ~Component() = default;
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;
		virtual void RenderImGui() = 0;
		dae::GameObject* GetOwner() const { return m_pOwner; }

	protected:
		dae::GameObject* m_pOwner;
	};
}