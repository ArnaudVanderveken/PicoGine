#include "GameObject.h"

#include "BaseComponent.h"

GameObject::~GameObject()
{
	for (const auto& component : m_pComponents)
		delete component;
}

void GameObject::Init()
{
}

void GameObject::FixedUpdate()
{
	for (const auto& component : m_pComponents)
		component->FixedUpdate();
}

void GameObject::Update()
{
	for (const auto& component : m_pComponents)
		component->Update();
}

void GameObject::LateUpdate()
{
	for (const auto& component : m_pComponents)
		component->LateUpdate();
}

void GameObject::Render()
{
	for (const auto& component : m_pComponents)
		component->Render();
}

void GameObject::MarkForDelete()
{
	m_MarkedForDelete = true;
	for (const auto& child : m_pChildren)
		child->MarkForDelete();
}

bool GameObject::IsMarkedForDelete() const
{
	return m_MarkedForDelete;
}

void GameObject::AddComponent(BaseComponent* component)
{
	m_pComponents.emplace_back(component);
}
