#include "GameObject.h"

#include "BaseComponent.h"

using namespace DirectX;

GameObject::GameObject() noexcept
{
	m_pLocalTransform = std::make_unique<LocalTransform>(this);
	m_pWorldTransform = std::make_unique<Transform>(this);
}

GameObject::~GameObject()
{
	for (const auto& component : m_pComponents)
		delete component;
}

void GameObject::FixedUpdate() const
{
	for (const auto& component : m_pComponents)
		component->FixedUpdate();
}

void GameObject::Update()
{
	for (const auto& component : m_pComponents)
		component->Update();

	if (m_CheckForDirtyTransform && m_pLocalTransform->IsDirty())
	{
		m_CheckForDirtyTransform = false;
		PropagateDirtyTransform();
	}
}

void GameObject::LateUpdate() const
{
	for (const auto& component : m_pComponents)
		component->LateUpdate();
}

void GameObject::Render() const
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

void GameObject::SetParent(GameObject* parent)
{
	if (m_pParent)
		m_pParent->RemoveChild(this);

	m_pParent = parent;

	if (m_pParent)
		m_pParent->AddChild(this);
}

void GameObject::AddComponent(BaseComponent* component)
{
	m_pComponents.emplace_back(component);
}

Transform& GameObject::GetWorldTransform()
{
	if (m_DirtyWorldTransform)
		UpdateWorldTransform();

	return *m_pWorldTransform;
}

Transform& GameObject::GetLocalTransform()
{
	m_CheckForDirtyTransform = true;
	return *m_pLocalTransform;
}

void GameObject::AddChild(GameObject* child)
{
	m_pChildren.emplace_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	std::erase(m_pChildren, child);
}

void GameObject::UpdateWorldTransform()
{
	m_DirtyWorldTransform = false;

	if (m_pParent)
		m_pWorldTransform->SetTransform(XMMatrixMultiply(XMLoadFloat4x4(&m_pLocalTransform->GetTransform()), XMLoadFloat4x4(&m_pParent->GetWorldTransform().GetTransform())));
	
	else
		m_pWorldTransform->SetTransform(m_pLocalTransform->GetTransform());
}

void GameObject::PropagateDirtyTransform()
{
	m_DirtyWorldTransform = true;

	for (const auto& child : m_pChildren)
		child->PropagateDirtyTransform();
}
