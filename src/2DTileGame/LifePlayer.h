#pragma once

#include "Player.h"

class LifePlayer : public Player
{
public:
	LifePlayer(LPCWSTR name, LPCWSTR scriptName, LPCWSTR textrueFilename);
	~LifePlayer();

	// State
public:
	void InitState();
};
