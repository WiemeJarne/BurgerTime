#pragma once
#include "BaseEvent.h"
#include "ButtonComponent.h"

namespace dae
{
	class ButtonRemovedFromActiveSceneEvent final : public BaseEvent
	{
	public:
		ButtonRemovedFromActiveSceneEvent(ButtonComponent* pButtonComponent)
			: m_pButtonComponent{ pButtonComponent }
		{}

		ButtonComponent* GetButtonComponent() const { return m_pButtonComponent; }

	private:
		ButtonComponent* m_pButtonComponent;
	};
}