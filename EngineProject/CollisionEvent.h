#pragma once
#include "BaseEvent.h"
#include "GameObject.h"

namespace dae
{
	class CollisionEvent final : public BaseEvent
	{
	public:
		CollisionEvent(GameObject* pTriggeredObject, GameObject* pOtherGameObject)
			: m_pTriggeredGameObject( pTriggeredObject )
			, m_pOtherGameObject( pOtherGameObject )
		{}

		GameObject* GetTriggeredGameObject() const { return m_pTriggeredGameObject; }
		GameObject* GetOtherGameObject() const { return m_pOtherGameObject; }

	private:
		GameObject* m_pTriggeredGameObject;
		GameObject* m_pOtherGameObject;
	};

}