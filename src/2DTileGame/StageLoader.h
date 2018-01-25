#pragma once

#include <Windows.h>
#include <string>

class Component;
class Stage;

class StageLoader
{
public:
	StageLoader(Stage* stage);
	~StageLoader();

protected:
	Stage* _stage;

	int _npcCount;
	int _lifeNpcCount;

public:
	virtual void CreateComponents(std::wstring mapName);

	Component* CreateNPC();
	Component* CreateLifeNPC(LPCWSTR scriptName, LPCWSTR textureFilename);
};
