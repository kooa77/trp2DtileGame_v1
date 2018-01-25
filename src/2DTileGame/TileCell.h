#pragma once

#include <list>

class Component;

class TileCell
{
private:
	float _posX;
	float _posY;

	std::list<Component*> _componentList;
	std::list<Component*> _renderList;

	int _tileX;
	int _tileY;


public:
	TileCell(int tileX, int tileY);
	~TileCell();

	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);
	float GetPositionX();
	float GetPositionY();

	void AddComponent(Component* component, bool isRender);
	void RemoveComponent(Component* component);
	std::list<Component*> GetComponentList() { return _componentList;  }
	Component* GetBaseComponent() { return _componentList.front(); }

	bool CanMove();
	bool GetCollisionList(std::list<Component*>& collisionList);

	int GetTileX() { return _tileX; }
	int GetTileY() { return _tileY; }

	// Pathfinding
protected:
	bool _isPathfindingMark;
	float _pathfindingDistance;
	
	float _distanceWeight;
	float _distanceFromStart;

	TileCell* _prevPathfindingCell;

public:
	void InitPathfinding();

	bool IsPathfindingMark() { return _isPathfindingMark; }
	void Pathfinded() { _isPathfindingMark = true; }

	float GetDistanceFromStart() { return _distanceFromStart; }
	void SetDistanceFromStart(float distance) { _distanceFromStart = distance;  }
	float GetDistanceWeight() { return _distanceWeight;  }

	TileCell* GetPrevPathfindingCell();
	void SetPrevPathfindingCell(TileCell* tileCell);
};
