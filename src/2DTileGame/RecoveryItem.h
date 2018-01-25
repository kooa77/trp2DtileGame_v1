#pragma once

#include <string>
#include "Component.h"

class Sprite;

class RecoveryItem : public Component
{
private:
	Sprite* _sprite;

	float _x;
	float _y;

	std::wstring _textrureFilename;
	std::wstring _scriptFilename;

public:
	RecoveryItem(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textureFilename);
	~RecoveryItem();

	void Init();
	void Deinit();
	void Update(float deltaTime);
	void Render();
	void Release();
	void Reset();

	void SetPosition(float posX, float posY);
	void MoveDeltaPosition(float deltaX, float deltaY);
	
	void ReceiveMessage(const sComponentMsgParam& msgParam);
};