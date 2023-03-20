#include "BaseComponent.h"


GameObject* BaseComponent::GetOwner() const
{
	return m_pOwner;
}

bool BaseComponent::IsActive() const
{
	return m_IsActive;
}

void BaseComponent::SetActive(bool active)
{
	m_IsActive = active;
}

void BaseComponent::TransformHasChanged()
{
	m_TransformHasChanged = true;
}

void BaseComponent::SetOwner(GameObject* owner)
{
	m_pOwner = owner;
}
