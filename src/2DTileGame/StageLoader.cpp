#include "Stage.h"
#include "Map.h"
#include "NPC.h"
#include "LifeNPC.h"

#include "StageLoader.h"


StageLoader::StageLoader(Stage* stage)
{
	_stage = stage;
	_npcCount = 0;
	_lifeNpcCount = 0;
}

StageLoader::~StageLoader()
{

}

void StageLoader::CreateComponents(std::wstring mapName)
{
	Map* map = new Map(mapName.c_str());
	_stage->SetMap(map);
	_stage->AddStageComponent(map);
}

Component* StageLoader::CreateNPC()
{
	WCHAR name[256];
	wsprintf(name, L"npc_%d", _npcCount);
	_npcCount++;

	Component* npc = new NPC(name, L"npc", L"char_sprite_01");
	_stage->AddStageComponent(npc);

	return npc;
}

Component* StageLoader::CreateLifeNPC(LPCWSTR scriptName, LPCWSTR textureFilename)
{
	WCHAR name[256];
	wsprintf(name, L"lifenpc_%d", _lifeNpcCount);
	_lifeNpcCount++;

	//Component* npc = new LifeNPC(name, L"npc", L"char_sprite_01");
	Component* npc = new LifeNPC(name, scriptName, textureFilename);
	_stage->AddStageComponent(npc);

	return npc;
}
