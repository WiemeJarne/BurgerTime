#ifndef NewSceneActivatedEvent_h
#define NewSceneActivatedEvent_h
#include "BaseEvent.h"
#include "Scene.h"

namespace dae
{
	class NewSceneActivatedEvent final : public BaseEvent
	{
	public:
		NewSceneActivatedEvent(Scene* pNewScene)
			: m_pNewlyActivatedScene { pNewScene }
		{}

		Scene* GetNewlyActivatedScene() const { return m_pNewlyActivatedScene; }

	private:
		Scene* m_pNewlyActivatedScene;
	};
}
#endif // !NewSceneActivatedEvent_h