#include "CollisionBoxComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"

dae::CollisionBoxComponent::CollisionBoxComponent(dae::GameObject* pOwner, float width, float height)
	: Component(pOwner)
	, m_Width{ width }
	, m_Height{ height }
{
	CollisionManager::GetInstance().AddCollider(this);
}

dae::CollisionBoxComponent::~CollisionBoxComponent()
{
	CollisionManager::GetInstance().RemoveCollider(this);
}

const glm::vec2& dae::CollisionBoxComponent::GetTopLeftPos() const
{ 
	return m_pOwner->GetLocalPos(); 
}

dae::GameObject* dae::CollisionBoxComponent::GetOwner() const
{
	return m_pOwner;
}