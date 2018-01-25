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

		// todo : �ش� ��ǥ Ÿ�ϼ������� �Ÿ��� ã�Ƴ��, ĳ������ ���ÿ� �ִ´�. (_pathTileCellStack). ���� ��ã�� �̵� ���·� �Ѿ��.
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

