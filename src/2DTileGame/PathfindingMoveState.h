#pragma once

#include <stack>
#include "State.h"

class Character;
class TileCell;

class PathfindingMoveState : public State
{
public:
	PathfindingMoveState();
	~PathfindingMoveState();

private:
	float _movingDuration;
	std::stack<TileCell*> _pathTileCellList;

public:
	void Init(Character* character);
	void Update(float deltaTime);

	void Start();
	void Stop();
};
