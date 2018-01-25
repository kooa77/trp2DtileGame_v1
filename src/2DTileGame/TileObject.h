#pragma once

#include "Component.h"

class Sprite;

class TileObject : public Component
{
private:
	Sprite* _sprite;

	float _posX;
	float _posY;

public:
	TileObject(LPCWSTR name, Sprite* sprite, int tileX, int tileY);
	virtual ~TileObject();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);

	// distance weight
private:
	float _distanceWeight;

public:
	void SetDistanceWeight(float weight) { _distanceWeight = weight;  }
	float GetDistanceWeight() { return _distanceWeight;  }
};