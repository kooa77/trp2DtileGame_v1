#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"

#include "IdleState.h"
#include "PathfindingImmediateState.h"
#include "PathfindingMoveState.h"

#include "PathfinderPlayer.h"


PathfinderPlayer::PathfinderPlayer(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) :
	Player(name, scriptName, textureFilename)
{
}

PathfinderPlayer::~PathfinderPlayer()
{
}

void PathfinderPlayer::UpdateAI(float deltaTime)
{
	if (GameSystem::GetInstance().IsMouseDown())
	{
		int mouseX = GameSystem::GetInstance().GetMouseX();
		int mouseY = GameSystem::GetInstance().GetMouseY();
		TileCell* targetTileCell = GameSystem::GetInstance().GetStage()->GetMap()->GetTileCellWithPixelPos(mouseX, mouseY);

		// todo : 해당 목표 타일셀까지의 거리를 찾아내어서, 캐릭터의 스택에 넣는다. (_pathTileCellStack). 이후 길찾기 이동 상태로 넘어난다.
		SetTargetCell(targetTileCell);
	}
}

void PathfinderPlayer::InitState()
{
	Player::InitState();

	ReplaceState(eStateType::ET_PATHFINDING, new PathfindingImmediateState());

	ReplaceState(eStateType::ET_MOVE, new PathfindingMoveState());
	//ReplaceState(eStateType::ET_ATTACK, new IdleState());
}

void PathfinderPlayer::SetTargetCell(TileCell* tileCell)
{
	_targetTileCell = tileCell;
	_state->NextState(eStateType::ET_PATHFINDING);
}

