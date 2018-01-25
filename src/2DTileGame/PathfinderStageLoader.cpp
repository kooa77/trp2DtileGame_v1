#include "Stage.h"
#include "Map.h"
#include "PathfinderMonster.h"
#include "PathfinderPlayer.h"

#include "PathfinderStageLoader.h"

PathfinderStageLoader::PathfinderStageLoader(Stage* stage) : StageLoader(stage)
{
}

PathfinderStageLoader::~PathfinderStageLoader()
{
}

void PathfinderStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);
	
	WCHAR name[256];
	int monsterCount = 0;
	wsprintf(name, L"pathfindermonster_%d", monsterCount);

	Component* monster = new PathfinderMonster(name, L"monster", L"char_sprite_02");
	_stage->AddStageComponent(monster);

	PathfinderPlayer* player = new PathfinderPlayer(L"player", L"player", L"player");
	_stage->AddStageComponent(player);
	
	int midTileX = _stage->GetMap()->GetWidth() / 2;
	int midTileY = _stage->GetMap()->GetHeight() / 2;
	std::list<Component*> componentList = _stage->GetMap()->GetTileComponentList(midTileX, midTileY);

	_stage->GetMap()->InitViewer(componentList.front());
}
