#include "GlobalTypes.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Character.h"
#include "RecoveryItem.h"
#include "PathfinderPlayer.h"

#include "PathfindingState.h"


PathfindingState::PathfindingState()
{
	_targetTileCell = NULL;
	_reverseTileCell = NULL;
	_updateState = eUpdateState::PATHFINDING;
}

PathfindingState::~PathfindingState()
{
}

void PathfindingState::Init(Character* character)
{
	State::Init(character);
}

void PathfindingState::Update(float deltaTime)
{
	State::Update(deltaTime);

	if (eStateType::ET_NONE != _nextState)
	{
		_character->ChangeState(_nextState);
		return;
	}

	switch (_updateState)
	{
	case eUpdateState::PATHFINDING:
		UpdatePathfinding();
		break;
	case eUpdateState::BUILD_PATH:
		UpdateBuildPath();
		break;
	}
}

void PathfindingState::UpdatePathfinding()
{
	if (0 != _pathfindingTileQueue.size())
	{
		sPathCommand command = _pathfindingTileQueue.top();
		_pathfindingTileQueue.pop();

		if (false == command.tileCell->IsPathfindingMark())
		{
			command.tileCell->Pathfinded();

			if (command.tileCell->GetTileX() == _targetTileCell->GetTileX() &&
				command.tileCell->GetTileY() == _targetTileCell->GetTileY())
			{
				std::list<Component*> componentList = _targetTileCell->GetComponentList();
				for (std::list<Component*>::iterator it = componentList.begin();
					it != componentList.end(); it++)
				{
					if (eComponentType::CT_MONSTER == (*it)->GetType())
					{
						Character* monster = (Character*)(*it);
						if (command.tileCell->GetTileX() < command.tileCell->GetPrevPathfindingCell()->GetTileX())
						{
							monster->SetDirection(eDirection::RIGHT);
						}
						else if (command.tileCell->GetPrevPathfindingCell()->GetTileX() < command.tileCell->GetTileX())
						{
							monster->SetDirection(eDirection::LEFT);
						}
						else if (command.tileCell->GetTileY() < command.tileCell->GetPrevPathfindingCell()->GetTileY())
						{
							monster->SetDirection(eDirection::DOWN);
						}
						else if (command.tileCell->GetPrevPathfindingCell()->GetTileY() < command.tileCell->GetTileY())
						{
							monster->SetDirection(eDirection::UP);
						}
					}
				}

				// 목표에 도달
				OutputDebugString(L"Find Goal\n");
				_updateState = eUpdateState::BUILD_PATH;
				_reverseTileCell = _targetTileCell;
				return;
			}

			for (int direction = 0; direction < eDirection::NONE; direction++)
			{
				TilePosition currentTilePos;
				currentTilePos.x = command.tileCell->GetTileX();
				currentTilePos.y = command.tileCell->GetTileY();
				TilePosition nextTilePos = GetNextTilePosition(currentTilePos, (eDirection)direction);

				Map* map = GameSystem::GetInstance().GetStage()->GetMap();
				TileCell* nextTileCell = map->GetTileCell(nextTilePos);
				if ((true == map->CanMoveTileMap(nextTilePos) && false == nextTileCell->IsPathfindingMark()) ||
					(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY())
					)	// 검사 해야 할 타일 셀이면 (갈 수 없어도, 목표 셀이면 포함한다.)
				{
					float distance = command.tileCell->GetDistanceFromStart() + nextTileCell->GetDistanceWeight();
					
					float heuristic = CalcAStarHeuristic(distance, nextTileCell, _targetTileCell);

					if (NULL == nextTileCell->GetPrevPathfindingCell())		// 세팅된 이전 셀이 없으면
					{
						nextTileCell->SetDistanceFromStart(distance);
						
						nextTileCell->SetPrevPathfindingCell(command.tileCell);
						sPathCommand newCommand;
						newCommand.heuristic = heuristic;
						newCommand.tileCell = nextTileCell;
						_pathfindingTileQueue.push(newCommand);

						// check 2
						// for test
						/*
						if (
							!(nextTileCell->GetTileX() == _targetTileCell->GetTileX() && nextTileCell->GetTileY() == _targetTileCell->GetTileY()) &&
							!(nextTileCell->GetTileX() == _character->GetTileX() && nextTileCell->GetTileY() == _character->GetTileY())
							)
						{
							GameSystem::GetInstance().GetStage()->CreatePathfinderNPC(nextTileCell);
						}
						*/
					}
					else
					{
						if (distance < nextTileCell->GetDistanceFromStart())		// 계산된 거리가 더 짧으면
						{
							// 다시 검사를 한다.
							nextTileCell->SetDistanceFromStart(distance);
							
							float newHeuristic = CalcAStarHeuristic(distance, nextTileCell, _targetTileCell);

							nextTileCell->SetPrevPathfindingCell(command.tileCell);
							sPathCommand newCommand;
							newCommand.heuristic = heuristic;
							newCommand.tileCell = nextTileCell;
							_pathfindingTileQueue.push(newCommand);
						}
					}
				}
			}
		}
	}
}

void PathfindingState::UpdateBuildPath()
{
	if (NULL != _reverseTileCell)
	{
		// check 3
		// for test
		//GameSystem::GetInstance().GetStage()->CreatePathfindingMark(_reverseTileCell);
		_character->PushPathTileCell(_reverseTileCell);
		_reverseTileCell = _reverseTileCell->GetPrevPathfindingCell();
	}
	else
	{
		_nextState = eStateType::ET_MOVE;
	}
}

void PathfindingState::Start()
{
	State::Start();

	_targetTileCell = ((PathfinderPlayer*)_character)->GetTargetCell();

	// 모든 타일 오브젝트의 길찾기  변수를 초기화 한다.
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	int height = map->GetHeight();
	int width = map->GetWidth();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			TileCell* tileCell = map->GetTileCell(x, y);
			tileCell->InitPathfinding();
		}
	}

	TileCell* startTileCell = map->GetTileCell(_character->GetTileX(), _character->GetTileY());
	
	sPathCommand newCommand;
	newCommand.heuristic = 0.0f;
	newCommand.tileCell = startTileCell;
	_pathfindingTileQueue.push(newCommand);

	_updateState = eUpdateState::PATHFINDING;
}

void PathfindingState::Stop()
{
	State::Stop();

	while (0 != _pathfindingTileQueue.size())
	{
		_pathfindingTileQueue.pop();
	}
}

float PathfindingState::CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell)
{
	float heuristic = 0.0f;

	int diffFromCurrent;
	int diffFromNext;

	// x 축
	{
		// 현재 타일부터 목표 까지의 거리를 구한다.
		diffFromCurrent = tileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromCurrent < 0)
			diffFromCurrent = -diffFromCurrent;

		// 검사할 타일 부터 목표 까지의 거리를 구한다.
		diffFromNext = nextTileCell->GetTileX() - targetTileCell->GetTileX();
		if (diffFromNext < 0)
			diffFromNext = -diffFromNext;

		if (diffFromNext < diffFromCurrent)		// 검사할 타일이 현재 타일보다 목표와 더 가까우면
		{
			heuristic -= 1.0f;
		}
		else if (diffFromCurrent < diffFromNext)	// 현재 타일이 검사할 타일보다 목표와 더 가까우면
		{
			heuristic += 1.0f;
		}
	}

	// y 축
	{
		// 현재 타일부터 목표 까지의 거리를 구한다.
		diffFromCurrent = tileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromCurrent < 0)
			diffFromCurrent = -diffFromCurrent;

		// 검사할 타일 부터 목표 까지의 거리를 구한다.
		diffFromNext = nextTileCell->GetTileY() - targetTileCell->GetTileY();
		if (diffFromNext < 0)
			diffFromNext = -diffFromNext;

		if (diffFromNext < diffFromCurrent)		// 검사할 타일이 현재 타일보다 목표와 더 가까우면
		{
			heuristic -= 1.0f;
		}
		else if (diffFromCurrent < diffFromNext)	// 현재 타일이 검사할 타일보다 목표와 더 가까우면
		{
			heuristic += 1.0f;
		}
	}

	return heuristic;
}

float PathfindingState::CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell)
{
	int distanceW = nextTileCell->GetTileX() - targetTileCell->GetTileX();
	int distanceH = nextTileCell->GetTileY() - targetTileCell->GetTileY();

	distanceW = distanceW * distanceW;
	distanceH = distanceH * distanceH;

	return (float)((double)distanceW + (double)distanceH);
}

float PathfindingState::CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell)
{
	return distanceFromStart + CalcComplexHeuristic(nextTileCell, targetTileCell);
}
