#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "MoveState.h"
#include "NPC.h"


NPC::NPC(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename) : Character(name, scriptName, textrueFilename)
{
	_type = eComponentType::CT_NPC;

	/*
	int speed = (rand() % 500) + 500;
	_moveTime = (float)speed / 1000.0f;
	*/
	_moveTime = 1.5f;
}

NPC::~NPC()
{

}

void NPC::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	Character::ReceiveMessage(msgParam);
	
	if (L"Meet" == msgParam.message)
	{
		//NPC* newNpc = GameSystem::GetInstance().CreateNPC();
		//newNpc->Init(GetTileX(), GetTileY());
	}
}

void NPC::UpdateAI(float deltaTime)
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	std::vector<eComponentType> compareTypeList;
	compareTypeList.push_back(eComponentType::CT_NPC);
	compareTypeList.push_back(eComponentType::CT_PLAYER);
	Component* findEnemy = ComponentSystem::GetInstance().FindComponentInRange(map, this, 4, compareTypeList);

	if (NULL != findEnemy)
	{
		// 추격 방향 설정
		eDirection direction = eDirection::NONE;
		for (int findDir = 0; findDir < 4; findDir++)
		{
			int newTileX = _tileX;
			int newTileY = _tileY;

			switch (findDir)
			{
			case eDirection::LEFT:
				newTileX--;
				break;
			case eDirection::RIGHT:
				newTileX++;
				break;
			case eDirection::UP:
				newTileY--;
				break;
			case eDirection::DOWN:
				newTileY++;
				break;
			}

			//Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"Map");
			//Map* map = GameSystem::GetInstance().GetStage()->GetMap();
			if (map->CanMoveTileMap(newTileX, newTileY))
			{
				_currentDirection = (eDirection)findDir;
				_state->NextState(eStateType::ET_MOVE);
				break;
			}
		}
	}
	else
	{
		Character::UpdateAI(deltaTime);
	}
}