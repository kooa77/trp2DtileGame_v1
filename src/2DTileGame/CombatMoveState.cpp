#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Character.h"
#include "CombatMoveState.h"

CombatMoveState::CombatMoveState()
{

}

CombatMoveState::~CombatMoveState()
{

}

void CombatMoveState::Init(Character* character)
{
	State::Init(character);
	_movingDuration = 0.0f;
}

void CombatMoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;

	if (false == _character->IsMoving())
		return;

	if (_character->GetMoveTime() <= _movingDuration)
	{
		_movingDuration = 0.0f;
		_character->MoveStop();
		_nextState = eStateType::ET_IDLE;
	}
	else
	{
		_movingDuration += deltaTime;
		_character->Moving(deltaTime);
	}
}

void CombatMoveState::Start()
{
	State::Start();

	_nextState = eStateType::ET_NONE;

	if (true == _character->IsMoving())
		return;

	//Map* map = (Map*)ComponentSystem::GetInstance().FindComponent(L"Map");
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();

	int newTileX = _character->GetTileX();
	int newTileY = _character->GetTileY();
	switch (_character->GetDirection())
	{
	case eDirection::LEFT:	// left
		newTileX--;
		break;
	case eDirection::RIGHT:	// right
		newTileX++;
		break;
	case eDirection::UP:	// up
		newTileY--;
		break;
	case eDirection::DOWN:	// down
		newTileY++;
		break;
	}

	std::list<Component*> collisionList;
	bool canMove = map->GetTileCollisionList(newTileX, newTileY, collisionList);
	if (false == canMove)
	{
		Component* target = _character->Collision(collisionList);
		if (NULL != target && _character->IsAttackCooltime())
		{
			_character->ResetAttackCooltime();
			_character->SetTarget(target);
			_nextState = eStateType::ET_ATTACK;
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_character->MoveStart(newTileX, newTileY);
	}
}

void CombatMoveState::Stop()
{
	State::Stop();
}
