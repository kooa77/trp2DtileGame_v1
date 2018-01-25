#include "Map.h"
#include "ComponentSystem.h"

ComponentSystem* ComponentSystem::_instance = 0;


ComponentSystem& ComponentSystem::GetInstance()
{
	if (0 == _instance)
	{
		_instance = new ComponentSystem();
	}
	return *_instance;
}

ComponentSystem::ComponentSystem()
{
	_componentMap.clear();
}

ComponentSystem::~ComponentSystem()
{

}

void ComponentSystem::Update(float deltaTime)
{
	ProcessMessageQueue();
}

void ComponentSystem::AddComponent(std::wstring name, Component* component)
{
	if (NULL != component)
	{
		std::map < std::wstring, Component*>::iterator it = _componentMap.find(name);
		if (it == _componentMap.end())
		{
			_componentMap[name] = component;
		}
	}
}

void ComponentSystem::RemoveAllComponents()
{
	for (std::map < std::wstring, Component*>::iterator it = _componentMap.begin();
		it != _componentMap.end();
		it++)
	{
		delete it->second;
	}
	_componentMap.clear();
}

void ComponentSystem::RemoveComponent(Component* component)
{
	_componentMap.erase(component->GetName());
	delete component;
}

Component* ComponentSystem::FindComponent(std::wstring name)
{
	std::map < std::wstring, Component*>::iterator it = _componentMap.find(name);
	if (it != _componentMap.end())
	{
		return it->second;
	}
	return 0;
}

Component* ComponentSystem::FindComponentInRange(Component* mapComp, Component* chaser, int range, std::vector<eComponentType> compareTypeList)
{
	Map* map = (Map*)mapComp;

	int minTileX = chaser->GetTileX() - range;
	int maxTileX = chaser->GetTileX() + range;
	int minTileY = chaser->GetTileY() - range;
	int maxTileY = chaser->GetTileY() + range;

	if (minTileX < 0)
		minTileX = 0;
	if (map->GetWidth() <= maxTileX)
		maxTileX = map->GetWidth() - 1;
	if (minTileY < 0)
		minTileY = 0;
	if (map->GetHeight() <= maxTileY)
		maxTileY = map->GetHeight() - 1;

	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			std::list<Component*> componentList;
			if (false == map->GetTileCollisionList(x, y, componentList))
			{
				for (std::list<Component*>::iterator it = componentList.begin();
					it != componentList.end(); it++)
				{
					Component* component = (*it);
					if (true == component->IsLive())
					{
						for (int i = 0; i < compareTypeList.size(); i++)
						{
							if (compareTypeList[i] == component->GetType())
							{
								return component;
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

void ComponentSystem::SendMsg(const sComponentMsgParam& msgParam)
{
	//msgParam.receiver->ReceiveMessage(msgParam);
	_msgQueue.push(msgParam);
}

void ComponentSystem::ProcessMessageQueue()
{
	while (0 < _msgQueue.size())
	{
		sComponentMsgParam msgParam = _msgQueue.front();
		_msgQueue.pop();
		msgParam.receiver->ReceiveMessage(msgParam);
	}
}

void ComponentSystem::ClearMessageQueue()
{
	while (0 < _msgQueue.size())
	{
		_msgQueue.pop();
	}
}



