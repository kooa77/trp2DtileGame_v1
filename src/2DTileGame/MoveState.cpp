#include "GameSystem.h"
#include "ComponentSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Character.h"
#include "MoveState.h"

MoveState::MoveState()
{

}

MoveState::~MoveState()
{

}

void MoveState::Init(Character* character)
{
	State::Init(character);
	_movingDuration = 0.0f;
}

void MoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (false == _character->IsLive())
		return;

	if (_character->GetMoveTime() <= _movingDuration)
	{
		_nextState = eStateType::ET_IDLE;
	}
	else
	{
		_movingDuration += deltaTime;
	}
}

void MoveState::Start()
{
	State::Start();

	_nextState = eStateType::ET_NONE;

	if (true == _character->IsMoving())
		return;

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
			_nextState = eStateType::ET_MEET;
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_character->MoveStart(newTileX, newTileY);
		_character->MoveStop();
		_movingDuration = 0.0f;
	}
}

void MoveState::Stop()
{
	State::Stop();
}
