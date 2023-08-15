#include "DamageComponent.h"
#include <iostream>

DamageComponent::DamageComponent(dae::GameObject* pOwner, int damageAmount)
	: Component(pOwner)
	, m_DamageAmount{ damageAmount }
{}

int DamageComponent::GetDamageAmount() const
{
	if (m_CanDoDamage)
		return m_DamageAmount;
	
	return 0;
}