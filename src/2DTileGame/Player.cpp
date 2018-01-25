#include "ComponentSystem.h"
#include "GameSystem.h"

#include "Stage.h"
#include "Map.h"

#include "CombatMoveState.h"
#include "IdleState.h"
#include "AttackState.h"
#include "DefenseState.h"
#include "DeadState.h"

#include "Player.h"


Player::Player(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename) : Character(name, scriptName, textrueFilename)
{
	_type = eComponentType::CT_PLAYER;
	_moveTime = 0.1f;
	_attackPoint = 50;
}

Player::~Player()
{

}

void Player::UpdateAI(float deltaTime)
{
	eDirection direction = eDirection::NONE;
	if (GameSystem::GetInstance().IsKeyDown(VK_UP))
	{
		direction = eDirection::UP;
	}
	if (GameSystem::GetInstance().IsKeyDown(VK_DOWN))
	{
		direction = eDirection::DOWN;
	}
	if (GameSystem::GetInstance().IsKeyDown(VK_LEFT))
	{
		direction = eDirection::LEFT;
	}
	if (GameSystem::GetInstance().IsKeyDown(VK_RIGHT))
	{
		direction = eDirection::RIGHT;
	}

	if (eDirection::NONE != direction)
	{
		_currentDirection = direction;
		_state->NextState(eStateType::ET_MOVE);
	}

	if (GameSystem::GetInstance().IsKeyDown(VK_SPACE))
	{
		Map* map = GameSystem::GetInstance().GetStage()->GetMap();
		std::list<Component*> componentList = map->GetTileComponentList(_tileX, _tileY);
		for (std::list<Component*>::iterator it = componentList.begin();
			it != componentList.end(); it++)
		{
			Component* component = (*it);
			if (eComponentType::CT_ITEM == component->GetType())
			{
				sComponentMsgParam msgParam;
				msgParam.sender = (Component*)this;
				msgParam.receiver = component;
				msgParam.message = L"Use";
				ComponentSystem::GetInstance().SendMsg(msgParam);
			}
		}
	}
}

void Player::InitState()
{
	ReplaceState(eStateType::ET_IDLE, new IdleState());
	ReplaceState(eStateType::ET_MOVE, new CombatMoveState());
	ReplaceState(eStateType::ET_ATTACK, new AttackState());
	ReplaceState(eStateType::ET_DEFENSE, new DefenseState());
	ReplaceState(eStateType::ET_DEAD, new DeadState());
}

Component*  Player::Collision(std::list<Component*>& collisionList)
{
	for (std::list<Component*>::iterator it = collisionList.begin(); it != collisionList.end(); it++)
	{
		Component* com = (*it);
		if (com->GetType() == eComponentType::CT_MONSTER || 
			com->GetType() == eComponentType::CT_NPC )
		{
			return (*it);
		}
	}
	return NULL;
}
