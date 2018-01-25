#pragma once

#include "Monster.h"

class PathfinderMonster : public Monster
{
public:
	PathfinderMonster(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename);
	~PathfinderMonster();

	// AI
public:
	void UpdateAI(float deltaTime);
};
