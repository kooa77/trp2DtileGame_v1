#pragma once

#include <queue>
#include "TileCell.h"
#include "State.h"

class Character;
class TileCell;

class PathfindingState : public State
{
public:
	PathfindingState();
	~PathfindingState();

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();

	// Pathfinding
protected:
	enum eUpdateState
	{
		PATHFINDING,
		BUILD_PATH
	};

	typedef struct _sPathCommand
	{
		float heuristic;
		TileCell* tileCell;
	} sPathCommand;

	struct compare
	{
		bool operator()(sPathCommand& a, sPathCommand& b)
		{
			return a.heuristic > b.heuristic;		// �� ����  ���� �տ� ������ ����
		}
	};

protected:
	std::priority_queue<sPathCommand, std::vector<sPathCommand>, compare> _pathfindingTileQueue;
	
	TileCell* _targetTileCell;
	TileCell* _reverseTileCell;
	eUpdateState _updateState;

protected:
	void UpdatePathfinding();
	void UpdateBuildPath();

	float CalcSimpleHeuristic(TileCell* tileCell, TileCell* nextTileCell, TileCell* targetTileCell);	
	float CalcComplexHeuristic(TileCell* nextTileCell, TileCell* targetTileCell);
	float CalcAStarHeuristic(float distanceFromStart, TileCell* nextTileCell, TileCell* targetTileCell);
};
