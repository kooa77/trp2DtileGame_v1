#include "ComponentSystem.h"
#include "TileCell.h"
#include "Map.h"
#include "LifeNPC.h"
#include "LifePlayer.h"
#include "NPC.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"
#include "LifeStageLoader.h"
#include "PathfinderStageLoader.h"
#include "DefaultStageLoader.h"
#include "Stage.h"


Stage::Stage()
{
	_loader = NULL;
}

Stage::~Stage()
{
	ComponentSystem::GetInstance().RemoveAllComponents();
	
	for (std::map<std::wstring, StageLoader*>::iterator it = _loaderMap.begin(); it != _loaderMap.end(); it++)
	{
		delete it->second;
	}
	_loaderMap.clear();
}

void Stage::Init(std::wstring mapName)
{
	_loaderMap[L"Default"] = new DefaultStageLoader(this);
	_loaderMap[L"Map3"] = new LifeStageLoader(this);
	_loaderMap[L"Map4"] = new PathfinderStageLoader(this);

	_componentList.clear();

	_loader = GetStageLoader(mapName);
	_loader->CreateComponents(mapName);
}

void Stage::Update(float deltaTime)
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Update(deltaTime);
	}

	UpdateBaseComponentList();
	UpdateRemoveComponentList();
}

void Stage::Render()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Render();
	}
}

void Stage::Release()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Release();
	}
}

void Stage::Reset()
{
	for (std::list<Component*>::iterator it = _componentList.begin(); it != _componentList.end(); it++)
	{
		(*it)->Reset();
	}
}

void Stage::AddStageComponent(Component* component)
{
	component->Init();
	_componentList.push_back(component);
}

StageLoader* Stage::GetStageLoader(std::wstring name)
{
	std::map<std::wstring, StageLoader*>::iterator it = _loaderMap.find(name);
	if (it != _loaderMap.end())
	{
		return it->second;
	}
	
	return _loaderMap[L"Default"];
}

Map* Stage::GetMap()
{
	return _map;
}

void Stage::SetMap(Map* map)
{
	_map = map;
}

void Stage::CreatePathfinderNPC(TileCell* tileCell)
{
	Component* baseComponent = tileCell->GetBaseComponent();
	LifeNPC* npc = (LifeNPC*)(_loader->CreateLifeNPC(L"npc", L"char_sprite_01"));
	npc->InitTilePosition(baseComponent->GetTileX(), baseComponent->GetTileY());
	npc->SetCanMove(true);

	_componentList.remove(npc);
	tileCell->AddComponent(npc, true);
	
	if (tileCell->GetTileX() < tileCell->GetPrevPathfindingCell()->GetTileX())
	{
		npc->SetDirection(eDirection::RIGHT);
	}
	else if (tileCell->GetPrevPathfindingCell()->GetTileX() < tileCell->GetTileX())
	{
		npc->SetDirection(eDirection::LEFT);
	}
	else if (tileCell->GetTileY() < tileCell->GetPrevPathfindingCell()->GetTileY())
	{
		npc->SetDirection(eDirection::DOWN);
	}
	else if (tileCell->GetPrevPathfindingCell()->GetTileY() < tileCell->GetTileY())
	{
		npc->SetDirection(eDirection::UP);
	}
}

void Stage::CreatePathfindingMark(TileCell* tileCell)
{
	Component* baseComponent = tileCell->GetBaseComponent();
	LifeNPC* npc = (LifeNPC*)(_loader->CreateLifeNPC(L"monster", L"char_sprite_02"));
	npc->InitTilePosition(baseComponent->GetTileX(), baseComponent->GetTileY());
	npc->SetCanMove(true);

	_componentList.remove(npc);
	tileCell->AddComponent(npc, true);
	
	if (NULL != tileCell->GetPrevPathfindingCell())
	{
		if (tileCell->GetPrevPathfindingCell()->GetTileX() < tileCell->GetTileX())
		{
			npc->SetDirection(eDirection::RIGHT);
		}
		else if (tileCell->GetTileX() < tileCell->GetPrevPathfindingCell()->GetTileX())
		{
			npc->SetDirection(eDirection::LEFT);
		}
		else if (tileCell->GetPrevPathfindingCell()->GetTileY() < tileCell->GetTileY())
		{
			npc->SetDirection(eDirection::DOWN);
		}
		else if (tileCell->GetTileY() < tileCell->GetPrevPathfindingCell()->GetTileY())
		{
			npc->SetDirection(eDirection::UP);
		}
	}
}

void Stage::CreateLifeNPC(Component* component)
{
	_createBaseComponentList.push_back(component);
}

void Stage::DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter)
{
	_map->ResetTileComponent(tileX, tileY, tileCharacter);
	_componentList.remove(tileCharacter);
	ComponentSystem::GetInstance().RemoveComponent(tileCharacter);
}

void Stage::UpdateBaseComponentList()
{
	for (std::list<Component*>::iterator it = _createBaseComponentList.begin();
		it != _createBaseComponentList.end(); it++)
	{
		Component* baseComponent = (*it);
		LifeNPC* npc = (LifeNPC*)(_loader->CreateLifeNPC(L"npc", L"char_sprite_01"));
		npc->InitTilePosition(baseComponent->GetTileX(), baseComponent->GetTileY());
	}
	_createBaseComponentList.clear();
}

void Stage::UpdateRemoveComponentList()
{
	for (std::list<Component*>::iterator it = _removeComponentList.begin();
		it != _removeComponentList.end(); it++)
	{
		Component* com = (*it);
		DestroyLifeNPC(com->GetTileX(), com->GetTileY(), com);
	}
	_removeComponentList.clear();
}

void Stage::CheckDestroyLifeNPC(Component* component)
{
	_removeComponentList.push_back(component);
}
