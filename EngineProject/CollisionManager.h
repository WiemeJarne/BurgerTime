#pragma once
#include "Singleton.h"
#include <vector>
#include "glm/vec2.hpp"
#include "CollisionBoxComponent.h"
#include "CollisionEvent.h"
#include "EventQueue.h"

namespace dae
{
	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void Update();
		void AddCollider(CollisionBoxComponent* pCollisionBoxComponent);
		void RemoveCollider(const CollisionBoxComponent* pCollisionBoxComponent);

	private:
		std::vector<CollisionBoxComponent*> m_CollisionBoxComponents;

		bool AreColliding(const glm::vec2& collider1TopLeft, float collider1With, float collider1Height, const glm::vec2& collider2TopLeft, float collider2Width, float collider2Height);
	};
}