#include <fstream>

#include "GameSystem.h"
#include "TileCell.h"
#include "Sprite.h"
#include "TileObject.h"
#include "LifeTileObject.h"
#include "Map.h"


Map::Map(LPCWSTR name) : Component(name)
{
	_startX = _startY = _deltaX = _deltaY = 0.0f;
	_spriteList.clear();
}

Map::~Map()
{
}

void Map::Init()
{
	int srcX = 0;
	int srcY = 0;
	_tileSize = 32;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			Sprite* sprite = new Sprite(L"MapSprite.png", L"MapSprite.json");
			sprite->Init(srcX, srcY, _tileSize, _tileSize, 1.0f);
			_spriteList.push_back(sprite);
			srcX += _tileSize;
		}

		srcX = 0;
		srcY += _tileSize;
	}

	_width = 8;
	_height = 26;

	std::wstring wname = _name;
	std::string name = "";
	name.assign(wname.begin(), wname.end());

	char layer01Name[256];
	sprintf(layer01Name, "%sData_layer01.csv", name.c_str());

	char layer02Name[256];
	sprintf(layer02Name, "%sData_layer02.csv", name.c_str());

	// Load Map Script 1층
	{
		int line = 0;
		int row = 0;
		char record[1024*4];
		std::ifstream infile(layer01Name);
		while (!infile.eof())
		{
			infile.getline(record, 1024*4);

			char* token = strtok(record, ",");
			switch (line)
			{
			case 0:
				if (NULL != token)
				{
					token = strtok(NULL, ",");
					_width = atoi(token);
					token = strtok(NULL, ",");
					_height = atoi(token);
				}
				break;
			case 1:
				break;
			default:
				// map data
				if (NULL != token)
				{
					std::vector<TileCell*> rowList;
					for (int x = 0; x < _width; x++)
					{
						int index = atoi(token);						
						TileCell* tileCell = new TileCell(x, row);
						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer01_%d_%d", line, x);
						TileObject* tileObject = new TileObject(componentName, _spriteList[index], x, row);
						// for test
						switch(index)
						{
						case 23:
							tileObject->SetDistanceWeight(0.5f);
							break;
						case 69:
							tileObject->SetDistanceWeight(1.5f);
							break;
						default:
							tileObject->SetDistanceWeight(1.0f);
						}
						tileObject->SetCanMove(true);
						tileCell->AddComponent(tileObject, true);
						rowList.push_back(tileCell);
						token = strtok(NULL, ",");
					}
					_tileMap.push_back(rowList);
					row++;
				}
				break;
			}

			line++;
		}
	}

	// 2층을 로드
	{
		int line = 0;
		int row = 0;
		char record[1024 * 4];
		std::ifstream infile(layer02Name);
		while (!infile.eof())
		{
			infile.getline(record, 1024 * 4);

			char* token = strtok(record, ",");
			switch (line)
			{
			case 0:
				break;
			case 1:
				break;
			default:
				// map data
				if (NULL != token)
				{
					std::vector<TileCell*> rowList = _tileMap[row];
					for (int x = 0; x < _width; x++)
					{
						int index = atoi(token);
						TileCell* tileCell = rowList[x];
						WCHAR componentName[256];
						wsprintf(componentName, L"map_layer02_%d_%d", line, x);
						if (0 <= index)
						{
							if (100100 == index)
							{
								LifeTileObject* tileObject = new LifeTileObject(x, row, componentName, _spriteList[20]);
								tileObject->SetCanMove(true);
								tileCell->AddComponent(tileObject, true);
							}
							else
							{
								TileObject* tileObject = new TileObject(componentName, _spriteList[index], x, row);
								tileCell->AddComponent(tileObject, true);
							}
						}						
						token = strtok(NULL, ",");
					}
					row++;
				}
				break;
			}

			line++;
		}
	}
}

void Map::Deinit()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Deinit();
			delete _tileMap[y][x];
		}
	}
}

void Map::Update(float deltaTime)
{
	// 뷰어를 중심으로, 렌더링 할 영역을 구한다.

	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

	// 뷰어의 위치를 기준으로 시작 픽셀 위치를 계산

	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	// 해당위치에 타일을 그린다.
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			_tileMap[y][x]->Update(deltaTime);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
}

void Map::Render()
{
	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

	int minX = _viewer->GetTileX() - (midX / _tileSize) - 2;
	int maxX = _viewer->GetTileX() + (midX / _tileSize) + 2;
	int minY = _viewer->GetTileY() - (midX / _tileSize) - 2;
	int maxY = _viewer->GetTileY() + (midX / _tileSize) + 2;

	if (minX < 0)
		minX = 0;
	if (_width <= maxX)
		maxX = _width;
	if (minY < 0)
		minY = 0;
	if (_height <= maxY)
		maxY = _height;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			_tileMap[y][x]->Render();
		}
	}
}

void Map::Release()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Release();
		}
	}
}

void Map::Reset()
{
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->Reset();
		}
	}
}

void Map::Scroll(float deltaX, float deltaY)
{
	_deltaX = deltaX;
	_deltaY = deltaY;
}

int Map::GetPositionX(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetPositionX();
}

int Map::GetPositionY(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetPositionY();
}

void Map::SetTileComponent(int tileX, int tileY, Component* component, bool isRender)
{
	_tileMap[tileY][tileX]->AddComponent(component, isRender);
}

void Map::ResetTileComponent(int tileX, int tileY, Component* component)
{
	_tileMap[tileY][tileX]->RemoveComponent(component);
}

std::list<Component*> Map::GetTileComponentList(int tileX, int tileY)
{
	return _tileMap[tileY][tileX]->GetComponentList();
}

TileCell* Map::GetTileCell(TilePosition tilePos)
{
	return GetTileCell(tilePos.x, tilePos.y);
}

TileCell* Map::GetTileCell(int tileX, int tileY)
{
	return _tileMap[tileY][tileX];
}

TileCell* Map::GetTileCellWithPixelPos(int mouseX, int mouseY)
{
	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;

	int minX = _viewer->GetTileX() - (midX / _tileSize) - 2;
	int maxX = _viewer->GetTileX() + (midX / _tileSize) + 2;
	int minY = _viewer->GetTileY() - (midX / _tileSize) - 2;
	int maxY = _viewer->GetTileY() + (midX / _tileSize) + 2;

	if (minX < 0)
		minX = 0;
	if (_width <= maxX)
		maxX = _width;
	if (minY < 0)
		minY = 0;
	if (_height <= maxY)
		maxY = _height;

	for (int y = minY; y < maxY; y++)
	{
		for (int x = minX; x < maxX; x++)
		{
			RECT rect;
			rect.left = _tileMap[y][x]->GetPositionX() - _tileSize / 2.0f;
			rect.right = rect.left + _tileSize;
			rect.top = _tileMap[y][x]->GetPositionY() - _tileSize / 2.0f;
			rect.bottom = rect.top + _tileSize;
			if (rect.left <= mouseX && mouseX <= rect.right && rect.top <= mouseY && mouseY <= rect.bottom)
				return _tileMap[y][x];
		}
	}

	return NULL;
}

bool Map::CanMoveTileMap(TilePosition tilePosition)
{
	return CanMoveTileMap(tilePosition.x, tilePosition.y);
}

bool Map::CanMoveTileMap(int tileX, int tileY)
{
	if (tileX < 0)
		return false;
	if (_width <= tileX)
		return false;
	if (tileY < 0)
		return false;
	if (_height <= tileY)
		return false;

	return _tileMap[tileY][tileX]->CanMove();
}

bool Map::GetTileCollisionList(int tileX, int tileY, std::list<Component*>& collisionList)
{
	if (tileX < 0)
		return false;
	if (_width <= tileX)
		return false;
	if (tileY < 0)
		return false;
	if (_height <= tileY)
		return false;

	return _tileMap[tileY][tileX]->GetCollisionList(collisionList);
}

void Map::InitViewer(Component* viewer)
{
	_viewer = viewer;

	// 뷰어를 중심으로, 렌더링 할 영역을 구한다.

	int midX = GameSystem::GetInstance().GetClientWidth() / 2;
	int midY = GameSystem::GetInstance().GetClientHeight() / 2;
	
	// 뷰어의 위치를 기준으로 시작 픽셀 위치를 계산

	_startX = (-_viewer->GetTileX() * _tileSize) + midX - _tileSize / 2;
	_startY = (-_viewer->GetTileY() * _tileSize) + midY - _tileSize / 2;

	// 해당위치에 타일을 그린다.
	float posX = _startX;
	float posY = _startY;
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			_tileMap[y][x]->SetPosition(posX, posY);
			posX += _tileSize;
		}
		posX = _startX;
		posY += _tileSize;
	}
}

int Map::GetWidth()
{
	return _width;
}

int Map::GetHeight()
{
	return _height;
}

