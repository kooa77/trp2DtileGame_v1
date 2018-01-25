#include "Stage.h"
#include "Map.h"
#include "LifePlayer.h"

#include "LifeStageLoader.h"


LifeStageLoader::LifeStageLoader(Stage* stage) : StageLoader(stage)
{
	
}

LifeStageLoader::~LifeStageLoader()
{

}

void LifeStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);

	for (int i = 0; i < 100; i++)
	{
		CreateLifeNPC(L"npc", L"char_sprite_01");
	}

	Player* player = new LifePlayer(L"player", L"player", L"player");
	_stage->AddStageComponent(player);

	_stage->GetMap()->InitViewer(player);
}

