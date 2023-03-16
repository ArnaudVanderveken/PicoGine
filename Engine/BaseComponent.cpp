#include "BaseComponent.h"


void BaseComponent::FixedUpdate()
{
}

void BaseComponent::Update()
{
}

void BaseComponent::LateUpdate()
{
}

void BaseComponent::Render()
{
}

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

void BaseComponent::SetOwner(GameObject* owner)
{
	m_pOwner = owner;
}
