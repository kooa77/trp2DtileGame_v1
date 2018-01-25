#include "Stage.h"
#include "Map.h"
#include "Player.h"
#include "Monster.h"
#include "RecoveryItem.h"

#include "DefaultStageLoader.h"


DefaultStageLoader::DefaultStageLoader(Stage* stage) : StageLoader(stage)
{
}

DefaultStageLoader::~DefaultStageLoader()
{
}

void DefaultStageLoader::CreateComponents(std::wstring mapName)
{
	StageLoader::CreateComponents(mapName);

	for (int i = 0; i < 10; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"recovery_item_%d", i);
		RecoveryItem* item = new RecoveryItem(name, L"recovery_item", L"item_sprites");
		_stage->AddStageComponent(item);
	}

	for (int i = 0; i < 10; i++)
	{
		CreateNPC();
	}

	for (int i = 0; i < 20; i++)
	{
		WCHAR name[256];
		wsprintf(name, L"monster_%d", i);
		Monster* monster = new Monster(name, L"monster", L"char_sprite_02");
		_stage->AddStageComponent(monster);
	}

	Player* player = new Player(L"player", L"player", L"player");
	_stage->AddStageComponent(player);

	_stage->GetMap()->InitViewer(player);
}
