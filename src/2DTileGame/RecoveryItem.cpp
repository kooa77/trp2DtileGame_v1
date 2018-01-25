#include "ComponentSystem.h"
#include "GameSystem.h"
#include "Stage.h"
#include "Map.h"
#include "Character.h"
#include "Sprite.h"
#include "RecoveryItem.h"


RecoveryItem::RecoveryItem(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename) : Component(name)
{
	_type = eComponentType::CT_ITEM;
	_sprite = NULL;
	_scriptFilename = scriptName;
	_textrureFilename = textureFilename;
}

RecoveryItem::~RecoveryItem()
{
}

void RecoveryItem::Init()
{
	Map* map = GameSystem::GetInstance().GetStage()->GetMap();
	int tileX = rand() % (map->GetWidth() - 1) + 1;
	int tileY = rand() % (map->GetHeight() - 1) + 1;
	while (1)
	{
		if (true == map->CanMoveTileMap(tileX, tileY))
		{
			_tileX = tileX;
			_tileY = tileY;
			break;
		}
		tileX = rand() % (map->GetWidth() - 1) + 1;
		tileY = rand() % (map->GetHeight() - 1) + 1;
	}

	_x = map->GetPositionX(_tileX, _tileY);
	_y = map->GetPositionY(_tileX, _tileY);
	map->SetTileComponent(_tileX, _tileY, this, false);

	WCHAR textrureFilename[256];
	wsprintf(textrureFilename, L"%s.png", _textrureFilename.c_str());

	WCHAR scriptFilename[256];
	{
		wsprintf(scriptFilename, L"%s.json", _scriptFilename.c_str());
		_sprite = new Sprite(textrureFilename, scriptFilename);
		_sprite->Init();
	}

	_canMove = true;
}

void RecoveryItem::Deinit()
{
	_sprite->Deinit();
}

void RecoveryItem::Update(float deltaTime)
{
	if (false == _isLive)
		return;

	_sprite->Update(deltaTime);
}

void RecoveryItem::Render()
{
	if (false == _isLive)
		return;

	_sprite->SetPosition(_x, _y);
	_sprite->Render();
}

void RecoveryItem::Release()
{
	_sprite->Release();
}

void RecoveryItem::Reset()
{
	_sprite->Reset();
}

void RecoveryItem::SetPosition(float posX, float posY)
{
	_x = posX;
	_y = posY;
}

void RecoveryItem::MoveDeltaPosition(float deltaX, float deltaY)
{
	_x += deltaX;
	_y += deltaY;
}

void RecoveryItem::ReceiveMessage(const sComponentMsgParam& msgParam)
{
	if (L"Use" == msgParam.message)
	{
		Component* sender = msgParam.sender;
		switch (sender->GetType())
		{
		case eComponentType::CT_NPC:
		case eComponentType::CT_MONSTER:
		case eComponentType::CT_PLAYER:
			((Character*)sender)->IncreaseHP(100);
			Map* map = GameSystem::GetInstance().GetStage()->GetMap();
			map->ResetTileComponent(_tileX, _tileY, this);
			_isLive = false;
			break;
		}
	}
}


