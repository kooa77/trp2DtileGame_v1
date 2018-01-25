#include "Character.h"
#include "PathfindingImmediateState.h"


PathfindingImmediateState::PathfindingImmediateState()
{

}

PathfindingImmediateState::~PathfindingImmediateState()
{

}

// check 1 - �ѹ��� ����ϰ�, ���� ������Ʈ�� �Ѿ��.
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
