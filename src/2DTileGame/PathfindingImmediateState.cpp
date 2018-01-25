#include "Character.h"
#include "PathfindingImmediateState.h"


PathfindingImmediateState::PathfindingImmediateState()
{

}

PathfindingImmediateState::~PathfindingImmediateState()
{

}

// check 1 - 한번에 계산하고, 다음 스테이트로 넘어간다.
void PathfindingImmediateState::Start()
{
	PathfindingState::Start();

	while (0 != _pathfindingTileQueue.size())
	{
		if (eUpdateState::BUILD_PATH == _updateState)
			break;

		UpdatePathfinding();
	}

	while (eStateType::ET_MOVE != _nextState)
	{
		UpdateBuildPath();
	}
}
