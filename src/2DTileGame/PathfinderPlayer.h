#pragma once

#include <queue>
#include "Player.h"

class TileCell;

class PathfinderPlayer : public Player
{
public:
	PathfinderPlayer(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename);
	~PathfinderPlayer();

	// AI
public:
	void UpdateAI(float deltaTime);

	// State
public:
	void InitState();

private:
	std::queue<TileCell*> _pathfindTileQueue;
	TileCell* _targetTileCell;

public:
	TileCell* GetTargetCell() { return _targetTileCell; }
	void SetTargetCell(TileCell* tileCell);
};
