#pragma once

#include "StageLoader.h"

class LifeStageLoader : public StageLoader
{
public:
	LifeStageLoader(Stage* stage);
	~LifeStageLoader();

public:
	void CreateComponents(std::wstring mapName);
};
