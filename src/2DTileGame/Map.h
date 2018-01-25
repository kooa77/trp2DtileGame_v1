#pragma once

#include <Windows.h>
#include <vector>
#include <list>
#include "GlobalTypes.h"
#include "Component.h"

class TileCell;
class Sprite;

class Map : public Component
{
private:
	std::vector<std::vector<TileCell*>> _tileMap;
	int _width;
	int _height;

	float _startX;
	float _startY;
	float _deltaX;
	float _deltaY;

	int _tileSize;

	Component* _viewer;

	std::vector<Sprite*> _spriteList;

public:
	Map(LPCWSTR mapName);
	~Map();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void Scroll(float deltaX, float deltaY);

	int GetPositionX(int tileX, int tileY);
	int GetPositionY(int tileX, int tileY);

	void SetTileComponent(int tileX, int tileY, Component* component, bool isRender);
	void ResetTileComponent(int tileX, int tileY, Component* component);
	std::list<Component*> GetTileComponentList(int tileX, int tileY);
	
	TileCell* GetTileCell(TilePosition tilePos);
	TileCell* GetTileCell(int tileX, int tileY);
	TileCell* GetTileCellWithPixelPos(int x, int y);

	bool CanMoveTileMap(TilePosition tilePosition);
	bool CanMoveTileMap(int tileX, int tileY);
	bool GetTileCollisionList(int newTileX, int newTileY, std::list<Component*>& collisionList);

	void InitViewer(Component* viewer);

	int GetWidth();
	int GetHeight();
};