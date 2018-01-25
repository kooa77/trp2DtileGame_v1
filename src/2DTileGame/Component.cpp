#include "ComponentSystem.h"
#include "Component.h"

Component::Component(std::wstring name)
{
	_type = eComponentType::CT_NONE;
	_name = name;
	_canMove = false;
	ComponentSystem::GetInstance().AddComponent(_name, this);

	_moveDistancePerTimeX = 0.0f;
	_moveDistancePerTimeY = 0.0f;

	_isLive = true;
}

Component::~Component()
{
}

void Component::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Collision" == msgParam.message)
	{
	}
}
