#include "ComponentSystem.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "MoveState.h"
#include "Monster.h"


Monster::Monster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename) :
	Character(name, scriptName, textrueFilename)
{
	_type = eComponentType::CT_MONSTER;

	_moveTime = 1.0f;
}

Monster::~Monster()
{

}

void Monster::UpdateAI(float deltaTime)
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
		if (findEnemy->GetTileX() < _tileX)
		{
			direction = eDirection::LEFT;
		}
		else if (_tileX < findEnemy->GetTileX())
		{
			direction = eDirection::RIGHT;
		}
		else if (findEnemy->GetTileY() < _tileY)
		{
			direction = eDirection::UP;
		}
		else if (_tileY < findEnemy->GetTileY())
		{
			direction = eDirection::DOWN;
		}

		if (eDirection::NONE != direction)
		{
			_currentDirection = direction;
			_state->NextState(eStateType::ET_MOVE);
		}
	}
	else
	{
		Character::UpdateAI(deltaTime);
	}
}

Component*  Monster::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator it = collisionList.begin(); it != collisionList.end(); it++)
	{
		Component* com = (*it);
		if (com->GetType() == eComponentType::CT_NPC ||
			com->GetType() == eComponentType::CT_PLAYER)
		{
			return (*it);
		}
	}
	return NULL;
}