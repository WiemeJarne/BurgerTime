#pragma once
#include "Component.h"

class DamageComponent final : public dae::Component
{
public:
	DamageComponent(dae::GameObject* pOwner, int damageAmount);

	void Update() {}
	void Render() const {}
	void RenderImGui() {}

	int GetDamageAmount() const;
	void SetCanDoDamage(bool canDoDamage) { m_CanDoDamage = canDoDamage; }

private:
	int m_DamageAmount;
	bool m_CanDoDamage{ true };
};