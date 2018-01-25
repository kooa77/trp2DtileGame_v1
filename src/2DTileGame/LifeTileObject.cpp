#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "LifeTileObject.h"

LifeTileObject::LifeTileObject(int tileX, int tileY, LPCWSTR name, Sprite* sprite) :
	TileObject(name, sprite, tileX, tileY)
{
}

LifeTileObject::~LifeTileObject()
{

}

void LifeTileObject::Update(float deltaTime)
{
	TileObject::Update(deltaTime);

	// 주변 8칸을 탐색 - 캐릭터가 몇마리 있는지
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	int range = 1;
	int minTileX = _tileX - range;
	int maxTileX = _tileX + range;
	int minTileY = _tileY - range;
	int maxTileY = _tileY + range;
	if (minTileX < 0) minTileX = 0;
	if (map->GetWidth() <= maxTileX) maxTileX = map->GetWidth() - 1;
	if (minTileY < 0) minTileY = 0;
	if (map->GetHeight() <= maxTileY) maxTileY = map->GetHeight() - 1;

	int surroundedCharacter = 0;
	//bool isTileCharacter = false;
	Component* tileCharacter = NULL;
	for (int y = minTileY; y <= maxTileY; y++)
	{
		for (int x = minTileX; x <= maxTileX; x++)
		{
			if (x != _tileX || y != _tileY)
			{
				std::list<Component*> componentList;
				if (false == map->GetTileCollisionList(x, y, componentList))
				{
					for (std::list<Component*>::iterator it = componentList.begin();
						it != componentList.end(); it++)
					{
						Component* componet = (*it);
						switch (componet->GetType())
						{
						case eComponentType::CT_NPC:
						case eComponentType::CT_PLAYER:
							surroundedCharacter++;
							break;
						}
					}
				}
			}
			else
			{
				std::list<Component*> componentList;
				if (false == map->GetTileCollisionList(x, y, componentList))
				{
					for (std::list<Component*>::iterator it = componentList.begin();
						it != componentList.end(); it++)
					{
						Component* component = (*it);
						switch (component->GetType())
						{
						case eComponentType::CT_NPC:
						case eComponentType::CT_PLAYER:
							//isTileCharacter = true;
							tileCharacter = component;
							break;
						}
					}
				}
			}
		}
	}

	if (3 == surroundedCharacter)
	{
		if(NULL == tileCharacter)
		{
			GameSystem::GetInstance().GetStage()->CreateLifeNPC(this);
		}
	}
	else if (2 == surroundedCharacter)
	{
		// skip
	}
	else
	{
		if (NULL != tileCharacter)
		{
			if (eComponentType::CT_PLAYER != tileCharacter->GetType())
			{
				//GameSystem::GetInstance().GetStage()->DestroyLifeNPC(_tileX, _tileY, tileCharacter);
				GameSystem::GetInstance().GetStage()->CheckDestroyLifeNPC(tileCharacter);
				tileCharacter = NULL;
			}
		}
	}
}
