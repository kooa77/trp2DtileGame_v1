#include "GlobalTypes.h"
#include "TileCell.h"
#include "Character.h"

#include "PathfindingMoveState.h"


PathfindingMoveState::PathfindingMoveState()
{

}

PathfindingMoveState::~PathfindingMoveState()
{

}

void PathfindingMoveState::Init(Character* character)
{
	State::Init(character);
}

void PathfindingMoveState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	if (_character->GetMoveTime() <= _movingDuration)
	{
		_movingDuration = 0.0f;

		if (0 != _pathTileCellList.size())
		{
			TileCell* tileCell = _pathTileCellList.top();
			_pathTileCellList.pop();

			TilePosition to;
			to.x = tileCell->GetTileX();
			to.y = tileCell->GetTileY();

			TilePosition from;
			from.x = _character->GetTileX();
			from.y = _character->GetTileY();

			eDirection direction = GetDirection(to, from);
			if (eDirection::NONE != direction)
			{
				_character->SetDirection(direction);
			}
			
			_character->MoveStart(tileCell->GetTileX(), tileCell->GetTileY());
			_character->MoveStop();
		}
		else
		{
			_nextState = eStateType::ET_IDLE;
		}
	}
	else
	{
		_movingDuration += deltaTime;
	}
}

void PathfindingMoveState::Start()
{
	State::Start();

	_movingDuration = 0.0f;

	_pathTileCellList = _character->GetPathTileCellStack();
	_pathTileCellList.pop();	// 첫번째는 자신의 위치이므로 빼버린다.
}

void PathfindingMoveState::Stop()
{
	State::Stop();

	_character->ClearPathTileCellStack();
}
