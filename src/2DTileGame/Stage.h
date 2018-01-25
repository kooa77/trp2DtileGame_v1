#pragma once

#include <string>
#include <list>
#include <map>

class Component;
class NPC;
class Map;
class StageLoader;
class TileCell;

class Stage
{
private:
	std::list<Component*> _componentList;

public:
	Stage();
	~Stage();

	void Init(std::wstring mapName);
	void Update(float deltaTime);
	void Render();

	void Release();
	void Reset();

	// StageLoader
private:
	std::map<std::wstring, StageLoader*> _loaderMap;
	StageLoader* _loader;

public:
	void AddStageComponent(Component* component);
	StageLoader* GetStageLoader(std::wstring name);

	// Map
private:
	Map* _map;

public:
	Map* GetMap();
	void SetMap(Map* map);

private:
	std::list<Component*> _createBaseComponentList;
	std::list<Component*> _removeComponentList;

public:
	void CreatePathfinderNPC(TileCell* tileCell);
	void CreatePathfindingMark(TileCell* tileCell);

	void CreateLifeNPC(Component* component);
	void DestroyLifeNPC(int tileX, int tileY, Component* tileCharacter);

	void UpdateBaseComponentList();
	void UpdateRemoveComponentList();
	void CheckDestroyLifeNPC(Component* component);
};
