#include "Sprite.h"
#include "TileObject.h"


TileObject::TileObject(LPCWSTR name, Sprite* sprite, int tileX, int tileY) : Component(name)
{
	_type = eComponentType::CT_TILE_OBJECT;
	_sprite = sprite;
	_tileX = tileX;
	_tileY = tileY;

	_distanceWeight = 1.0f;
}

TileObject::~TileObject()
{
}

void TileObject::Init()
{
}

void TileObject::Deinit()
{
	_sprite->Deinit();
}

void TileObject::Update(float deltaTime)
{
	_sprite->Update(deltaTime);
}

void TileObject::Render()
{
	_sprite->SetPosition(_posX, _posY);
	_sprite->Render();
}

void TileObject::Release()
{
	_sprite->Release();
}

void TileObject::Reset()
{
	_sprite->Reset();
}

void TileObject::SetPosition(float posX, float posY)
{
	_posX = posX;
	_posY = posY;
}

void TileObject::MoveDeltaPosition(float deltaX, float deltaY)
{
	_posX += deltaX;
	_posY += deltaY;
}
